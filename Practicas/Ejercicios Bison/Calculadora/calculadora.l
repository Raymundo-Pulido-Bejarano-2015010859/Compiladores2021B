%{
#include "calculadora.tab.h"
%}

%%
[0-9]+	{ return NUMERO; }
[*]		{ return '*'; }
[+]		{ return '+'; }
\n		{ return '\n'; }
[ ]*
.


%%
