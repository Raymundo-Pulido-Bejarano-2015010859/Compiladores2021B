%{
/*
	Herramienta: bison
		es la versión GNU de YACC
		YACC es la herramienta original de UNIX

		El lenguaje de llama YACC
		La extension de los archivos es .y

	Flex usa Expressiones regulares.
	Bison usa Gramáticas libres de contexto (GLC)
				Free Context Grammar (FCG)
En clase: 	minúscula para terminales
		  	mayúscula para no terminales

En Bison: 	mayúscula para terminales
			minúscula para no terminales
*/
#include <stdio.h>
/*Modulo de manejo de errores*/
void yyerror(char*mensaje){
	printf("Error: %s",mensaje);
}
%}
%token NUMERO ENTER
%%
entrada:
%empty
;
entrada: entrada linea
;
linea: ENTER
;
linea: expresion ENTER   { printf("Responder = ? \n"); }
;
expresion: NUMERO
;
expresion: expresion expresion '+'
;
expresion: expresion expresion '-'
;
expresion: expresion expresion '*'
;

%%
