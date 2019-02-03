%{
# include "exp5.tab.h"
%}

digit [0-9]
operator [-+*/]
%%
{digit}+    { yylval = atoi(yytext); return NUM; }
{operator}  { return yytext[0]; }

