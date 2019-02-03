%{
#include <stdio.h>
#include<iostream>
extern "C" void yyerror(const char *s);
extern int yylex(void);
extern int yylineno;


%}
%union{
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
};
%token <string_value> INTEGER  FLOAT VOID NAME 
%token ASSIGN 
%token <double_value> DOUBLE_NUMBER
%token <integer_value> INTEGER_NUMBER 
%right '='
%left '+' '-'
%left '*' '/'
%start PROGRAM
%nterm <symbol_table> GLOBAL_DECLARATIONS
%nterm <symbol_entry> GLOBAL_DECL
%nterm <>
//%nterm <procedure> MAIN_FUNCTION
%%

PROGRAM					: GLOBAL_DECLARATIONS MAIN_FUNCTION
							{
								//program_object.set_global_table(*$1);
								//program_object.set_procedure($2,yylineno);
								printf("%d",yylineno);
							}

GLOBAL_DECLARATIONS		: /* epsilon */
							{
								$$ = new Symbol_Table();
							}
						| GLOBAL_DECLARATIONS GLOBAL_DECL
							{
								$1->push_symbol($2);
								$$ = $1;
							}

GLOBAL_DECL 			: TYPE GLOBAL_NAME_LIST ';'
							{

							}

GLOBAL_NAME_LIST		: NAME | GLOBAL_NAME_LIST ',' NAME

TYPE					: INTEGER
						| FLOAT

MAIN_FUNCTION			: VOID NAME '(' ')' '{' PROCEDURE '}'

PROCEDURE				: LOCAL_DECLARATIONS STATEMENT_LIST

LOCAL_DECLARATIONS		: /* epsilon */
						| LOCAL_DECLARATIONS LOCAL_DECL

LOCAL_DECL 				: TYPE LOCAL_NAME_LIST ';'

LOCAL_NAME_LIST			: NAME | LOCAL_NAME_LIST ',' NAME

STATEMENT_LIST			: /* epsilon */
						| STATEMENT_LIST STATEMENT

STATEMENT				: NAME '=' EXPRESSION  ';'
							{printf("found a statement\n");}
						| NAME '=' FLOAT_EXPRESSION  ';'

EXPRESSION 				: INTEGER_NUMBER
							{printf("found an expression consisting of a number\n");}
						| NAME
							{printf("found an expression consisting of a identifier\n");}		
						| EXPRESSION  '+' EXPRESSION 
							{ printf("found a PLUS expression\n");}
						| EXPRESSION  '*' EXPRESSION 
							{ printf("found a MULT expression\n");}
						| EXPRESSION  '-' EXPRESSION 
							{ printf("found a SUB expression\n");}
						| EXPRESSION  '/' EXPRESSION 
							{ printf("found a DIV expression\n");}

FLOAT_EXPRESSION 		: DOUBLE_NUMBER
							{printf("found an expression consisting of a float number\n");}
						| NAME
							{printf("found an expression consisting of a identifier\n");}		
						| FLOAT_EXPRESSION  '+' FLOAT_EXPRESSION 
							{ printf("found a PLUS expression\n");}
						| FLOAT_EXPRESSION  '*' FLOAT_EXPRESSION 
							{ printf("found a MULT expression\n");}
						| FLOAT_EXPRESSION  '-' FLOAT_EXPRESSION 
							{ printf("found a SUB expression\n");}
						| FLOAT_EXPRESSION  '/' FLOAT_EXPRESSION 
							{ printf("found a DIV expression\n");}


%%

extern YYSTYPE yylval;