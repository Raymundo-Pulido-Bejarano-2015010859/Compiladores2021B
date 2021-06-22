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
	int numCond = 19;
	int FaltaPorCond = 0;
	int numgo = 20;
	int indicador = 15;

	char *aux = (char *) malloc(sizeof(char) * 50);
	int lenght = 50;
	symrec *sym_table;

	typedef struct Regla{
		int tipoRR ;		// 0 Difereccion, 1 constantes, 2 lineas de codigo,  5 vacio
		char *ValorT ;
		int tipoV;			// 0 int, 1 char,
		char *valAux; 
	}regla;

	symrec *getsymDir(char const *dir) {
	for (symrec *p = sym_table; p; p = p->next)
		if (strcmp (p->VarDir, dir) == 0)	
			return p;
	return NULL;
}

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
%nterm <regla> postfix_expression
%nterm <regla> primary_expression
%nterm <regla> multiplicative_expression
%nterm <regla> additive_expression
%nterm <regla> relational_expression
%nterm <regla> equality_expression
%nterm <regla> logical_and_expression
%nterm <regla> logical_or_expression
%nterm <regla> conditional_expression
%nterm <regla> assignment_expression
%nterm <regla> expression
%nterm <regla> constant_expression
%nterm <regla> declaration
%nterm <regla> init_declarator
%nterm <regla> type_specifier
%nterm <regla> declarator
%nterm <regla> direct_declarator
%nterm <regla> initializer
%nterm <regla> statement
%nterm <regla> compound_statement
%nterm <regla> declaration_list
%nterm <regla> statement_list
%nterm <regla> expression_statement
%nterm <regla> selection_statement
%nterm <regla> iteration_statement
%nterm <regla> jump_statement
%nterm <regla> translation_unit
%nterm <regla> external_declaration
%nterm <regla> function_definition

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
																	sprintf(aux,"if-lt %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nif-lt V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nif-lt V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nif-lt V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	FaltaPorCond++;
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| relational_expression '>' additive_expression			{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"if-gt %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nif-gt V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nif-gt V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nif-gt V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	FaltaPorCond++;
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| relational_expression LE_OP additive_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"if-le %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nif-le V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nif-le V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nif-le V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	FaltaPorCond++;
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| relational_expression GE_OP additive_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"if-ge %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nif-ge V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nif-ge V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nif-ge V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	FaltaPorCond++;
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

equality_expression
	: relational_expression									{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| equality_expression EQ_OP relational_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"if-eq %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nif-eq V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nif-eq V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nif-eq V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	FaltaPorCond++;
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	| equality_expression NE_OP relational_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"if-ne %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nif-ne V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nif-ne V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																	FaltaPorCond++;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nif-ne V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	FaltaPorCond++
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

logical_and_expression	
	: equality_expression									{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| logical_and_expression AND_OP equality_expression		{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"and-long %s,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nand-long V0,%s,:cond_%d",$3.ValorT,$1.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V0,%s\nand-long V0,%s,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nand-long V0,V1,:cond_%d",$1.ValorT,$3.ValorT,numCond+FaltaPorCond);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

logical_or_expression
	: logical_and_expression								{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| logical_or_expression OR_OP logical_and_expression	{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"or-long V0,%s,%s",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V1,%s\nor-long V0,%s,V1",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 1 && $3.tipoRR == 0){
																	sprintf(aux,"const-wide/16 V1,%s\nor-long V0,V1,%s \n",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nor-long V0,V0,V1 ",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

conditional_expression
	: logical_or_expression									{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	;

assignment_expression
	: conditional_expression								{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| postfix_expression '=' assignment_expression			{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"move-wide %s,%s \n",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"const-wide/16 V0,%s\nmove-wide %s,V0 \n",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\nmove-wide V1,V0\n%s\nmove-wide V1,V0 \n",$3.ValorT,$1.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;

expression
	: assignment_expression									{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	| expression ',' assignment_expression					{
																if($1.tipoRR == 0 && $3.tipoRR == 0){
																	sprintf(aux,"%s\n%s\n",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 0 && $3.tipoRR == 1){
																	sprintf(aux,"%s\n%s\n",$1.ValorT,$3.ValorT);
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}else if($1.tipoRR == 2 && $3.tipoRR == 2 ){
																	if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
																		lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
																		aux = (char*)realloc(aux,sizeof(char)*lenght);
																	}
																	sprintf(aux,"%s\n%s\n",$3.ValorT,$1.ValorT);
																	FaltaPorCond++;
																	$$.ValorT = strdup(aux);
																	$$.tipoRR = 2;
																}
															}
	;	

constant_expression
	: conditional_expression								{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; }
	;

declaration
	: type_specifier ';'					{ $$.tipoRR = 5; }
	| type_specifier init_declarator ';'	{ $$.ValorT = $2.ValorT; $$.tipoRR = $2.tipoRR; $$.tipoV = $1.tipoV;
												symrec* aux = getsymDir($$.valAux);
												aux->type = $1.tipoV;
											}
	;

init_declarator
	: declarator								{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; $$.tipoV = $1.tipoV;}
	| declarator '=' initializer 				{
													if($1.tipoRR == 0 && $3.tipoRR == 0){
														sprintf(aux,"move-wide %s,%s \n",$1.ValorT,$3.ValorT);
														$$.ValorT = strdup(aux);
														$$.tipoRR = 2;
														$$.valAux = strdup($1.ValorT);
													}else if($1.tipoRR == 0 && $3.tipoRR == 1){
														sprintf(aux,"const-wide/16 V0,%s\nmove-wide %s,V0 \n",$3.ValorT,$1.ValorT);
														$$.ValorT = strdup(aux);
														$$.tipoRR = 2;
														$$.valAux = strdup($1.ValorT);
													}else if($1.tipoRR == 0 && $3.tipoRR == 2 ){
														if (strlen($1.ValorT)+strlen($3.ValorT) > lenght){
															lenght = (strlen($1.ValorT)+strlen($3.ValorT)) *2;
															aux = (char*)realloc(aux,sizeof(char)*lenght);
														}
														sprintf(aux,"%s\nmove-wide V1,V0\n%s\nmove-wide V1,V0 \n",$3.ValorT,$1.ValorT);
														$$.ValorT = strdup(aux);
														$$.tipoRR = 2;
														$$.valAux = strdup($1.ValorT);
													}
												}
	;

type_specifier
	: CHAR										{ $$.tipoV = 1; $$.tipoRR = 5;}
	| INT 										{ $$.tipoRR = 2; $$.tipoRR = 5; }
	;

declarator 
	| direct_declarator							{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; $$.tipoV = $1.tipoV;}
	;

direct_declarator
	: IDENTIFIER										{ $$.ValorT = strdup($1->VarDir); $$.tipoRR = 0; }		
	| '(' declarator ')'								{ $$.ValorT = $2.ValorT; $$.tipoRR = $2.tipoRR; $$.tipoV = $2.tipoV;}
	| direct_declarator '[' ']'							{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; $$.tipoV = 1;}
	| direct_declarator '(' ')'							{ $$.ValorT = $1.ValorT; $$.tipoRR = $1.tipoRR; $$.tipoV = 3;}
	;

initializer
	: assignment_expression								{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	;

statement
	: compound_statement								{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| expression_statement								{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| selection_statement								{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| iteration_statement								{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| jump_statement									{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'							{ $$.ValorT = strdup($2.ValorT); $$.tipoRR = $2.tipoRR; }
	| '{' declaration_list '}'							{ $$.ValorT = strdup($2.ValorT); $$.tipoRR = $2.tipoRR; }
	| '{' declaration_list statement_list '}'			{ 
															if (strlen($2.ValorT)+strlen($3.ValorT) > lenght){
																lenght = (strlen($2.ValorT)+strlen($3.ValorT)) *2;
																aux = (char*)realloc(aux,sizeof(char)*lenght);
															}
															sprintf(aux,"%s\n%s\n",$2.ValorT,$3.ValorT);
															$$.ValorT = strdup(aux);
															$$.tipoRR = 2;
														}
	;

declaration_list
	: declaration										{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| declaration_list declaration						{ 
															if (strlen($1.ValorT)+strlen($2.ValorT) > lenght){
																lenght = (strlen($1.ValorT)+strlen($2.ValorT)) *2;
																aux = (char*)realloc(aux,sizeof(char)*lenght);
															}
															sprintf(aux,"%s\n%s\n",$1.ValorT,$2.ValorT);
															$$.ValorT = strdup(aux);
															$$.tipoRR = 2;
														}
	;

statement_list
	: statement											{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| statement_list statement							{ 
															if (strlen($1.ValorT)+strlen($2.ValorT) > lenght){
																lenght = (strlen($1.ValorT)+strlen($2.ValorT)) *2;
																aux = (char*)realloc(aux,sizeof(char)*lenght);
															}
															sprintf(aux,"%s\n%s\n",$1.ValorT,$2.ValorT);
															$$.ValorT = strdup(aux);
															$$.tipoRR = 2;
														}
	;

expression_statement
	: ';'
	| expression ';'						{	$$.ValorT = $2.ValorT; $$.tipoRR = $2.tipoRR;	}
	;

selection_statement
	: IF '(' expression ')' statement		{
												printf("%s\n",$3.ValorT);
												printf("%s\n",$5.ValorT);
												for(int i = 0; i < FaltaPorCond; i++){
													printf(":cond_%d",numCond+i);
												}
												FaltaPorCond = 0;
											}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{ 
												printf(":goto_%d",numgo);
											  	printf("%s\n",$3.ValorT);
												printf("%s\n",$5.ValorT);
												printf("goto :goto_%d",numgo);
												numgo++;
												for(int i = 0; i < FaltaPorCond; i++){
													printf(":cond_%d",numCond+i);
												}
												FaltaPorCond = 0;
											}
	;

jump_statement
	: BREAK ';'								{ 
												printf("goto %s", indicador); 
												printf("Opcode break");
											}
	| RETURN ';'							{ sprintf(aux,"return-void"); $$.ValorT = strdup(aux); $$.tipoRR = 2;}
	| RETURN expression ';'					{	if ( $2.tipoRR == 2 ){
													if (strlen($2.ValorT)+15 > lenght){
														lenght = (strlen($2.ValorT)) *2;
														aux = (char*)realloc(aux,sizeof(char)*lenght);
													}
													sprintf(aux,"%s\nmove-wide V1,V0\nreturn-wide V1\n",$2.ValorT);
													$$.ValorT = strdup(aux);
													$$.tipoRR = 2;
												}else if( $2.tipoRR == 0 ){
													sprintf(aux,"return-wide %s\n",$2.ValorT);
													$$.ValorT = strdup(aux);
													$$.tipoRR = 2;
												}else if( $2.tipoRR == 1 ){
													sprintf(aux,"const-wide/16 V0,%x \nreturn-wide V0\n",$2.ValorT);
													$$.ValorT = strdup(aux);
													$$.tipoRR = 2;
												}
											}
	;

translation_unit
	: external_declaration						{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| translation_unit external_declaration		{ 
													if (strlen($1.ValorT)+strlen($2.ValorT) > lenght){
														lenght = (strlen($1.ValorT)+strlen($2.ValorT)) *2;
														aux = (char*)realloc(aux,sizeof(char)*lenght);
													}
													sprintf(aux,"%s\n%s\n",$1.ValorT,$2.ValorT);
													$$.ValorT = strdup(aux);
													$$.tipoRR = 2;
												}
	;

external_declaration
	: function_definition						{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	| declaration								{ $$.ValorT = strdup($1.ValorT); $$.tipoRR = $1.tipoRR; }
	;

function_definition
	: type_specifier declarator declaration_list compound_statement		{ 	printf("\n%s\n",$4.ValorT);
																			for(int i = 0; i < FaltaPorCond; i++){
																				printf(":cond_%d",numCond+i);
																			}
																			FaltaPorCond = 0; 
																		}
	| type_specifier declarator compound_statement						{ 	printf("\n%s\n",$4.ValorT);
																			for(int i = 0; i < FaltaPorCond; i++){
																				printf(":cond_%d",numCond+i);
																			}
																			FaltaPorCond = 0; 
																		}
	| declarator declaration_list compound_statement					{ 	printf("\n%s\n",$4.ValorT);
																			for(int i = 0; i < FaltaPorCond; i++){
																				printf(":cond_%d",numCond+i);
																			}
																			FaltaPorCond = 0; 
																		}
	| declarator compound_statement										{ 	printf("\n%s\n",$4.ValorT);
																			for(int i = 0; i < FaltaPorCond; i++){
																				printf(":cond_%d",numCond+i);
																			}
																			FaltaPorCond = 0; 
																		}
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
