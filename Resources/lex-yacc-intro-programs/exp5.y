%{
#include <stdio.h>
%}
%token NUM
%left '+' '-'
%left '*' '/'
%%
E : NUM    
	{ printf("found an expression consisting of a number\n");}
  | E '+' E  
	{ printf("found a PLUS expression\n");}
  | E '*' E  
	{ printf("found a MULT expression\n");}
  | E '-' E  
	{ printf("found a SUB expression\n");}
  | E '/' E  
	{ printf("found a DIV expression\n");}
