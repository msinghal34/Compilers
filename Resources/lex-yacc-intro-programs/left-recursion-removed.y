%{
#include <stdio.h>
%}

%term NUM

%%

E : F T
  ;
T : '*' F T
  | 
  ;

F : NUM
  ;
%%

void yyerror(char *s)
{
}
