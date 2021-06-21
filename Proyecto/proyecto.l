
%{
#include <stdio.h>
#include <string.h>
#include "TabSimb.h"
#include "proyecto.tab.h"

void count();
int Vide = 2;
char auxVariable[50];
%}

D			[0-9]
L			[a-zA-Z_]

%%

"break"			{ count(); return(BREAK); }
"char"			{ count(); return(CHAR); }
"const"			{ count(); return(CONST); }
"if"			{ count(); return(IF); }
"int"			{ count(); return(INT); }
"return"		{ count(); return(RETURN); }
"while"			{ count(); return(WHILE); }

{L}({L}|{D})*		{ 	count(); 
						symrec *s = getsym( yytext );
						if (!s){
							s = putsym( yytext, 0 );
							sprintf(auxVariable,"V%d",Vide++);
							s->VarDir =  strdup(auxVariable);
						}

						yylval.IDENTIFIER = s;
						
						return(IDENTIFIER); 
					}
L?\"(\\.|[^\\"])*\"	{ 	count(); 
						yylval.STRING_LITERAL = strdum(yytext);
						return(STRING_LITERAL); 
					}
{D}+			   	{ 	count(); 
						yylval.CONSTANT = (int) atoi(yytext);;
						return(CONSTANT); 
					}

"&&"			{ count(); return(AND_OP); }
"||"			{ count(); return(OR_OP); }
"<="			{ count(); return(LE_OP); }
">="			{ count(); return(GE_OP); }
"=="			{ count(); return(EQ_OP); }
"!="			{ count(); return(NE_OP); }
";"			{ count(); return(';'); }
("{"|"<%")		{ count(); return('{'); }
("}"|"%>")		{ count(); return('}'); }
","			{ count(); return(','); }
"="			{ count(); return('='); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }
"&"			{ count(); return('&'); }
"!"			{ count(); return('!'); }
"-"			{ count(); return('-'); }
"+"			{ count(); return('+'); }
"*"			{ count(); return('*'); }
"/"			{ count(); return('/'); }
"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }
"["			{ count(); return('['); }
"]"			{ count(); return(']'); }
[ \t\v\n\f]		{ count(); }

%%

int yywrap() {
	return(1);
	//.			{ /* ignore bad characters */ }

}

int column = 0;

void count() {
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}

