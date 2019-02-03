%{
#include <stdio.h>
%}

%term NUM

%%

E : F T
  ;
T : '*' F T
  | F
  ;

F : NUM
  ;
%%

void yyerror(char *s)
{
}
