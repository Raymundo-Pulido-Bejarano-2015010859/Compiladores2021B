%{
	#include <stdio.h>

	extern int column;
	extern FILE *yyin;
	void yyerror(char const *s);
	int yylex(void);
	int numeroL = 15;
	int numCond = 20;
	int numgo = 20;
	int indicador = 15;
%}

%token IDENTIFIER CONSTANT STRING_LITERAL 
%token LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP 
%token CHAR INT CONST
%token IF WHILE RETURN BREAK
%nterm <char *> expression 
%nterm <char *> statement
%nterm <char *> primary_expression
%nterm <char *> postfix_expression
%nterm <char *> argument_expression_list
%nterm <char *> assignment_expression
%nterm <char *> multiplicative_expression
%nterm <char *> additive_expression
%nterm <char *> relational_expression
%nterm <char *> equality_expression
%nterm <char *> logical_and_expression
%nterm <char *> logical_or_expression
%nterm <char *> conditional_expression
%nterm <char *> constant_expression
%nterm <char *> declaration
%nterm <char *> init_declarator_list
%nterm <char *> init_declarator
%nterm <char *> type_specifier
%nterm <char *> declarator
%nterm <char *> direct_declarator
%nterm <char *> pointer
%nterm <char *> initializer
%nterm <char *> compound_statement
%nterm <char *> declaration_list
%nterm <char *> statement_list
%nterm <char *> expression_statement
%nterm <char *> selection_statement
%nterm <char *> iteration_statement
%nterm <char *> jump_statement


%start translation_unit

%%

primary_expression
	: IDENTIFIER
	| STRING_LITERAL
	| '(' expression ')'
	| CONSTANT
	;

postfix_expression
	: primary_expression
	| postfix_expression '(' argument_expression_list ')'
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

multiplicative_expression
	: postfix_expression
	| multiplicative_expression '*' postfix_expression
	| multiplicative_expression '/' postfix_expression
	| multiplicative_expression '%' postfix_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

relational_expression
	: additive_expression
	| relational_expression '<' additive_expression
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
