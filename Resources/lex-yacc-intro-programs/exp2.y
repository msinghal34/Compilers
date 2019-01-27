%{
#include <stdio.h>
%}
%token NUM
%%
E : NUM    
	{ printf("found an expression consisting of a number\n");}
  | E '+' E  
	{ printf("found a plus expression\n");}
