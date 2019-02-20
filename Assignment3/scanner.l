%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

char 			[a-zA-Z_]
digit 			[0-9]
operator 		[-+*/]
assignment 		[=]
identifier 		{char}({char}|{digit})*
float 			{digit}+\.{digit}+
meta			[,\{\}\(\)\;]
comment 		[/][/].*

%%

[ \t]+			{
				
			}

\n 				{

			}

int 			{
				store_token_name("INTEGER", yytext, yylineno);
				yylval.string_value = new string(yytext);
				return INTEGER;
			}

float 			{
				store_token_name("FLOAT", yytext, yylineno);
				yylval.string_value = new string(yytext);												
				return FLOAT;
			}
					
void		 	{
				store_token_name("VOID", yytext, yylineno);
				yylval.string_value = new string(yytext);								
				return VOID;
			}

{comment}		{
				;
			}
					
{identifier} 		{
				store_token_name("NAME", yytext, yylineno);
				yylval.string_value = new string(yytext);								
				return NAME;
			}
					
{operator}  		{
				store_token_name("ARITHOP", yytext, yylineno);
				yylval.string_value = new string(yytext);								
				return yytext[0];
			}
					
{assignment} 		{
				store_token_name("ASSIGN_OP", yytext, yylineno);
				yylval.string_value = new string(yytext);							
				return yytext[0];
			}
					
{meta}			{
				store_token_name("META CHAR", yytext, yylineno); 
				yylval.string_value = new string(yytext);								
				return yytext[0];
			}

{digit}+		{
				store_token_name("NUM", yytext, yylineno);
				yylval.integer_value = atoi(yytext);				
				return INTEGER_NUMBER;
			}					
			
{float}			{
				store_token_name("FNUM", yytext, yylineno);
				yylval.double_value = atof(yytext);					
				return DOUBLE_NUMBER;
			}

.           		{
				printf("%s\n", yytext);
				printf("\ncs316: Error\n");
				return -1;
			}
