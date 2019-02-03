%{
#include <stdio.h>
%}
%token NUM
%left '+'
%%
E : NUM    
	{ printf("found an expression consisting of a number with value %d\n", $1);}
  | E '+' E  
	{ 
		$$ = $1 + $3;
		printf("found a plus expression with the value %d\n", $1 + $3);
 	}
%%
int yyerror (char *mesg)
{
	fprintf (stderr, "%s\n", mesg);
}
