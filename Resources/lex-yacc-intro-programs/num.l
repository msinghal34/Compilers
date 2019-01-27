%{
#include <stdio.h>
%}

digit [0-9]
operator [-+*/]
%%
{digit}+    { printf ("found a number whose string is %s\n",yytext); }
{operator}  { printf ("found an operator %c\n", yytext[0]); }
.           ;
