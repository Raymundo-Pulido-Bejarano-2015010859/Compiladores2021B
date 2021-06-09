%{
#include "TabSimb.h"
#include "mfunc.tab.h"
%}
NUMBER		[0-9]+
DECIMAL 	("."{NUMBER})|({NUMBER}("."{NUMBER}?)?)
%%

[a-z]+[0-9a-zA-Z]*		{ 
							symrec *s = getsym( yytext );
							if (!s)
								s = putsym( yytext, 0 );
							printf("%i",VAR);							

							if (s->type){
								yylval.FUN = s;
								return FUN;
							} else { 
								yylval.VAR = s;
								return VAR;
							}
							
						}
{DECIMAL}				{ yylval.NUM = (double)atof( yytext ); return NUM; }
"="						{ return '='; }
"^"						{ return '^'; }
"*"						{ return '*'; }
"/"						{ return '/'; }
"+"						{ return '+'; }
"-"						{ return '-'; }
"("						{ return '('; }
")"						{ return ')'; }
\n					{ return '\n'; }
[ \t]					{ }


%%

