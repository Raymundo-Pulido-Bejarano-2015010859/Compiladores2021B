%{
    #include <stdio.h>  /* For printf, etc. */
    #include <math.h>   /* For pow, used in the grammar. */	
	#include "TabSimb.h"   /* Contains definition of 'symrec'. */
	#include <string.h>

	symrec *sym_table;	
	
	struct init {
		char const *name;
      	func_t *fun;
    };

    struct init const funs[] =
	{	
		{ "atan", atan },
        { "cos",  cos  },
        { "exp",  exp  },
        { "ln",   log  },
        { "sin",  sin  },
        { "sqrt", sqrt },
        { 0, 0 },
    };

	int yylex(void);
	void yyerror (char const *);
%}	

%define api.value.type union 	/* Generate YYSTYPE from these types: */
%token <double>  NUM     		/* Double precision number. */
%token <symrec*> VAR FUN 		/* Symbol table pointer: variable/function. */
%nterm <double>  exp			

%precedence '='
%left '-' '+'
%left '*' '/'
%precedence NEG 		/* negation--unary minus */
%right '^'      		/* exponentiation */


%% /* The grammar follows. */
input:
  %empty
| input line
;

line:
  '\n'		 
| exp '\n'   			{ printf ("\t%.10g\n",$1); 			}
| error '\n' 			{ yyerrok;                			}
;

exp:
  NUM				 
| VAR                	{ $$ = $1->value.var;               }
| VAR '=' exp        	{ $$ = $3; $1->value.var = $3;      }
| FUN '(' exp ')'    	{ $$ = $1->value.fun($3);     		}
| exp '+' exp        	{ $$ = $1 + $3;                     }
| exp '-' exp        	{ $$ = $1 - $3;                     }
| exp '*' exp        	{ $$ = $1 * $3;                     }
| exp '/' exp       	{ $$ = $1 / $3;                     }
| '-' exp  %prec NEG 	{ $$ = -$2;                         }
| exp '^' exp        	{ $$ = pow ($1, $3);                }
| '(' exp ')'        	{ $$ = $2;                          }
;
/* End of grammar. */

%%

/* Put functions in table. */
static void init_table (void) {
	for (int i = 0; funs[i].name; i++) {
		symrec *ptr = putsym (funs[i].name, 1);
        ptr->value.fun = funs[i].fun;
    }
}

/* Called by yyparse on error. */
void yyerror (char const *s) {
	fprintf (stderr, "%s\n", s);
}

int main (int argc, char const* argv[]) {
       /* Enable parse traces on option -p. */
    if (argc == 2 && strcmp(argv[1], "-p") == 0)
    	yydebug = 1;
    init_table ();
	return yyparse ();
}

