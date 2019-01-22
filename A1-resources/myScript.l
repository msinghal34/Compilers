

%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "storetokens.hh"
int linecount = 1;
%}

char 			[a-zA-Z_]
digit 			[0-9]
operator 		[-+*/]
assignment 		[=]
identifier 		{char}({char}|{digit})*
float 			{digit}*\.{digit}+
meta			[,\{\}\(\)\;]
comment 		[/][/].*	

%%
int 				{store_token_name("INTEGER", yytext, linecount);
					yylval.string_value = new string(yytext);
									
					return INTEGER;}
float 				{store_token_name("FLOAT", yytext, linecount);
					yylval.string_value = new string(yytext);												
					return FLOAT;}
					
void		 		{store_token_name("VOID", yytext, linecount);
					yylval.string_value = new string(yytext);								
					return VOID;}

{comment}			{/*printf("comment %s\n",yytext)*/;}
					
{identifier} 		{store_token_name("NAME", yytext, linecount);
					yylval.string_value = new string(yytext);								
					return NAME;}
					

{operator}  		{store_token_name("ARITHOP", yytext, linecount);
					yylval.string_value = new string(yytext);								
					return yytext[0];}
					
{assignment} 		{store_token_name("ASSIGN_OP", yytext, linecount);
					yylval.string_value = new string(yytext);							
					return ASSIGN;}
					
{meta}				{store_token_name("META CHAR", yytext, linecount); 
					yylval.string_value = new string(yytext);								
					return yytext[0];}

{digit}+			{store_token_name("NUM", yytext, linecount);
					yylval.integer_value = atoi(yytext);				
					return INTEGER_NUMBER;}					
					
{float}				{store_token_name("FNUM", yytext, linecount);
					yylval.double_value = atof(yytext);					
					return DOUBLE_NUMBER;}
\n 					{linecount++;}
.           		;
