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
float1 			{digit}*\.{digit}+
float2			{digit}+\.
float 			{float1}|{float2}
meta			[,\{\}\(\)\;\?:]
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

if 				{
					store_token_name("IF", yytext, yylineno);
					yylval.string_value = new string(yytext);												
					return IF;
				}

else 			{
					store_token_name("ELSE", yytext, yylineno);
					yylval.string_value = new string(yytext);												
					return ELSE;
				}

do 				{
					store_token_name("DO", yytext, yylineno);
					yylval.string_value = new string(yytext);												
					return DO;
				}

while 			{
					store_token_name("WHILE", yytext, yylineno);
					yylval.string_value = new string(yytext);												
					return WHILE;
				}

for 			{
					store_token_name("FOR", yytext, yylineno);
					yylval.string_value = new string(yytext);												
					return WHILE;
				}

					
void		 	{
					store_token_name("VOID", yytext, yylineno);
					yylval.string_value = new string(yytext);								
					return VOID;
				}

{comment}		{
					;
				}
					
{identifier} 	{
					store_token_name("NAME", yytext, yylineno);
					yylval.string_value = new string(yytext);								
					return NAME;
				}

"<="			{
					store_token_name("LESS_THAN_EQUAL", yytext, yylineno);
					yylval.relop_value = less_equalto;
					return RELOP;
				}

"<"				{
					store_token_name("LESS_THAN", yytext, yylineno);
					yylval.relop_value = less_than;
					return RELOP;
				}

">="			{
					store_token_name("GREATER_THAN_EQUAL", yytext, yylineno);
					yylval.relop_value = greater_equalto;
					return RELOP;
				}

">"				{
					store_token_name("GREATER_THAN", yytext, yylineno);
					yylval.relop_value = greater_than;
					return RELOP;
				}

"=="			{
					store_token_name("EQUAL", yytext, yylineno);
					yylval.relop_value = equalto;
					return EQOP;
				}

"!="			{
					store_token_name("NOT_EQUAL", yytext, yylineno);
					yylval.relop_value = not_equalto;
					return EQOP;
				}


"&&"			{
					store_token_name("AND", yytext, yylineno);
					yylval.string_value = new string(yytext);
					return AND;
				}

"||"			{
					store_token_name("OR", yytext, yylineno);
					yylval.string_value = new string(yytext);
					return OR;
				}

"!"				{
					store_token_name("NOT", yytext, yylineno);
					yylval.string_value = new string(yytext);
					return NOT;
				}

{operator}  	{
					store_token_name("ARITHOP", yytext, yylineno);
					yylval.string_value = new string(yytext);								
					return yytext[0];
				}
					
{assignment} 	{
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

.           	{
					printf("%s", yytext);
					printf("\ncs316: Error\nForeign Character Found\n");
					return -1;
				}
