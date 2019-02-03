%{
#include <stdio.h>
%}
%token NUM
%%
E : NUM    { printf("found an expression consisting of a number\n");}

%%
int yyerror (char *mesg)
{
	fprintf (stderr, "%s\n", mesg);
}
