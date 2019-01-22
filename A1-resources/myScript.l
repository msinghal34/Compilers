%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "storetokens.hh"
int mylinecount = 1;
%}

char 			[a-zA-Z_]
digit 			[0-9]
operator 		[-+*/]
assignment 		[=]
identifier 		{char}({char}|{digit})*
float 			{digit}*\.{digit}+
meta			[,\{\}\(\)\;]

%%
int 				{store_token_name("INTEGER", yytext, mylinecount);
					yylval.string_value = new string(yytext);
									
					return INTEGER;}
float 			{store_token_name("FLOAT", yytext, mylinecount);
					yylval.string_value = new string(yytext);												
					return FLOAT;}
					
void		 		{store_token_name("VOID", yytext, mylinecount);
					yylval.string_value = new string(yytext);								
					return VOID;}
					
{identifier} 	{store_token_name("NAME", yytext, mylinecount);
					yylval.string_value = new string(yytext);								
					return NAME;}
					

{operator}  	{store_token_name("ARITHOP", yytext, mylinecount);
					yylval.string_value = new string(yytext);								
					return yytext[0];}
					
{assignment} 	{store_token_name("ASSIGN_OP", yytext, mylinecount);
					yylval.string_value = new string(yytext);							
					return ASSIGN;}
					
{meta}			{store_token_name("META CHAR", yytext, mylinecount); 
					yylval.string_value = new string(yytext);								
					return yytext[0];}

{digit}+			{store_token_name("NUM", yytext, mylinecount);
					yylval.integer_value = atoi(yytext);				
					return INTEGER_NUMBER;}					
					
{float}			{store_token_name("FNUM", yytext, mylinecount);
					yylval.double_value = atof(yytext);					
					return DOUBLE_NUMBER;}
\n 				{mylinecount++;}
.           	;
