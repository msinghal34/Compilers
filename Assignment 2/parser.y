%{
#include <stdio.h>
extern "C" void yyerror(const char *s);
extern int yylex(void);
%}
%token INTEGER FLOAT VOID NAME ASSIGN INTEGER_NUMBER DOUBLE_NUMBER
%right '='
%left '+' '-'
%left '*' '/'
%start PROGRAM
%%

PROGRAM					: GLOBAL_DECLARATIONS MAIN_FUNCTION
							{printf("HI\n");}

GLOBAL_DECLARATIONS		: /* epsilon */
						| GLOBAL_DECLARATIONS GLOBAL_DECL

GLOBAL_DECL 			: TYPE GLOBAL_NAME_LIST ';'

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