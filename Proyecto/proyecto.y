%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "TabSimb.h"   /* Contains definition of 'symrec'. */
	#include <string.h>

	extern int column;
	extern FILE *yyin;
	void yyerror(char const *s);
	int yylex(void);

	int numeroL = 15;
	int numCond = 20;
	int numgo = 20;
	int indicador = 15;

	char *aux = (char *) malloc(sizeof(char) * 50);
	int lenght = 50;
	symrec *sym_table;

	typedef struct Regla{
		int tipoRR ;		// 0 Difereccion, 1 constantes, 2 lineas de codigo
		char *ValorT ;
	}regla;

%}

%define api.value.type union
%token <int> CONSTANT
%token <char*> STRING_LITERAL 
%token <symrec*> IDENTIFIER

%token LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP 
%token CHAR INT CONST
%token IF WHILE RETURN BREAK


%nterm <regla> expression 
%nterm <regla> primary_expression



%precedence '='
%left '-' '+'
%left '*' '/'

%start translation_unit

%%

primary_expression
	: IDENTIFIER			{	$$.ValorT = strdup($1->VarDir);
								$$.tipoRR = 0;
						 	}
	| STRING_LITERAL		{ 	$$.ValorT = strdup($1); $$.tipoRR = 1;	}
	| '(' expression ')'	{	$$.ValorT = $2.ValorT; $$.tipoRR = $2.tipoRR;	}
	| CONSTANT				{ 	sprintf(aux,"%x",$1);
								$$.ValorT = strdup(aux);
								$$.tipoRR = 1;
							}
	;

postfix_expression
	: primary_expression	{  
								$$.ValorT = $1.ValorT;
								$$.tipoRR = $1.tipoRR;
	 						}
	| postfix_expression '(' assignment_expression ')'  	{	
																sprintf(aux,"%s\n%s",$1.ValorT,$2.ValorT);
																$$.ValorT = strdup(aux);
																$$.tipoRR = 2;
															}
	;

multiplicative_expression
	: postfix_expression									{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| multiplicative_expression '*' postfix_expression		{ 
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"mul-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nmul-long V0,%s,V0",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nmul-long V0,%s,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nmul-long V0,V1,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}

															}
	| multiplicative_expression '/' postfix_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"div-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\ndiv-long V0,%s,V0",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\ndiv-long V0,%s,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\ndiv-long V0,V1,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| multiplicative_expression '%' postfix_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"rem-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nrem-long V0,%s,V0",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nrem-long V0,%s,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nrem-long V0,V1,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

additive_expression
	: multiplicative_expression								{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| additive_expression '+' multiplicative_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"add-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nadd-long V0,%s,V0",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nadd-long V0,%s,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nadd-long V0,V1,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| additive_expression '-' multiplicative_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"sub-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nsub-long V0,%s,V0",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nsub-long V0,%s,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nsub-long V0,V1,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

relational_expression
	: additive_expression									{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| relational_expression '<' additive_expression			{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"cmp-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\ncmp-long V0,%s,V0",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\ncmp-long V0,%s,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\ncmp-long V0,V1,V0",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| relational_expression '>' additive_expression
	| relational_expression LE_OP additive_expression
	| relational_expression GE_OP additive_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

logical_and_expression
	: equality_expression
	| logical_and_expression AND_OP equality_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	;

assignment_expression
	: conditional_expression
	| postfix_expression '=' assignment_expression
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;

declaration
	: type_specifier ';'
	| type_specifier init_declarator_list ';'
	;

init_declarator_list
	: init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer 				{
													printf("Crear la variable que se esta declarando");
													printf("Calcular la expresion con la que se inicializara");
													printf("Mover el resultado de la expresion al registro de la variable");
												}
	;

type_specifier
	: CHAR										{ printf("const-string");}
	| INT 										{ printf("const-wide/16");}
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER								{
													printf("Identificador Tabla de simbolos");
												}		
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' ')'
	;

pointer
	: '*'
	| '*' CONST
	| '*' pointer
	| '*' CONST pointer
	;


initializer
	: assignment_expression
	;

statement
	: compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	| '{' declaration_list statement_list '}'
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement
	| statement_list statement
	;

expression_statement
	: ';'
	| expression ';'						{		
												//printf("%s",$1);
												printf("expresion");	
											}
	;

selection_statement
	: IF '(' expression ')' statement		{
												//printf(".line %d",numeroL);
												//printf("%s",$3);		// lineas Condicion
												//printf("%s",$5);		// lineas de statement
												printf("lineas Condicion");
												printf("lineas de statement");
												printf(".line %d",numeroL);
												printf(":cond_%d",numCond);
											}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{ 
											  printf(".line %d", numeroL);
											  printf(":goto_%d",numgo); 
											  //printf("%s",$3);		//Incerto lineas de comparacion
											  //printf("%s",$5);		//Incerto lineas de statements
											  printf("Incerto lineas de comparacion");
											  printf("Incerto lineas de statements");
											  printf(".line %d",numeroL);
											  printf(":cond_%d",numCond);
											}
	;

jump_statement
	: BREAK ';'								{ 
												//printf("goto %s", indicador); 
												printf("Opcode break");
											}
	| RETURN ';'							{ printf("return-void"); }
	| RETURN expression ';'					{ 
												//printf("return-wide %s ",$1); 
												printf("Opcode del return");
											}
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: type_specifier declarator declaration_list compound_statement
	| type_specifier declarator compound_statement
	| declarator declaration_list compound_statement
	| declarator compound_statement
	;

%%


void yyerror(char const *s) {
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}

int main( int argc, char **argv){
	++argv, --argc; /* skip over program name */
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else	
		yyin = stdin;
	return yyparse();
}
