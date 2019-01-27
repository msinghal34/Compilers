%{
# include "y.tab.h"
%}

digit [0-9]
operator [-+*/]
%%
{digit}+    { return NUM; }
{operator}  { return yytext[0]; }

