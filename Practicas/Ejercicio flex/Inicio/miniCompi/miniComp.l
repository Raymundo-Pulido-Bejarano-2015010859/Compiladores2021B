%{
	#include <stdio.h>
%}

%%
[0-9]+						{printf("<numero>");}
int|char|float|void			{printf("<palabra reservada>");}
[a-zA-Z][a-zA-Z0-9]*		{printf("<id>");}
[-+]						{printf("<operador>");}
[;]							{printf("<separador>");}
