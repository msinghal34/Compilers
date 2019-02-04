%{
#include <stdio.h>
#include<iostream>
#include<string>
extern "C" void yyerror(const char *s);
extern int yylex(void);
extern int yylineno;

Data_Type curr_data_type;
string curr_proc_name;
Data_Type curr_proc_type;

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
%token <double_value> DOUBLE_NUMBER
%token <integer_value> INTEGER_NUMBER 
%right '='
%left '+' '-'
%left '*' '/'
%start PROGRAM 
%nterm <symbol_table> GLOBAL_DECLARATIONS LOCAL_DECLARATIONS
%nterm <symbol_table> DECLARATION
%nterm <symbol_table> NAME_LIST
%nterm <string_value> TYPE
%nterm <procedure> MAIN_FUNCTION PROCEDURE
%%

PROGRAM					: GLOBAL_DECLARATIONS MAIN_FUNCTION
							{
								program_object.set_global_table(*$1);
								program_object.set_procedure($2,yylineno);
								// printf("%d",yylineno);
							}

GLOBAL_DECLARATIONS		: /* epsilon */
							{
								$$ = new Symbol_Table();
							}
						| GLOBAL_DECLARATIONS DECLARATION
							{
								$1->append_list(*$2,yylineno);
								$$ = $1;
								$$->set_table_scope(global);
							}

DECLARATION				: TYPE NAME_LIST ';'
							{
								$$ = $2;
							}

NAME_LIST				: NAME
							{
								$$ = new Symbol_Table();
								$$->push_symbol(new Symbol_Table_Entry(*$1,curr_data_type,yylineno));
								
							}
						| NAME_LIST ',' NAME
							{
								$1->push_symbol(new Symbol_Table_Entry(*$3,curr_data_type,yylineno));
								$$ = $1;
							}
TYPE					: INTEGER 
							{
								curr_data_type = int_data_type;
								$$ = $1;
							}
						| FLOAT
							{
								curr_data_type = double_data_type;
								$$ = $1;
							}

DEF						: VOID NAME
							{
								curr_proc_type = void_data_type;
								curr_proc_name = *$2;
							}

MAIN_FUNCTION			: DEF '(' ')' '{' PROCEDURE '}'
							{
								// if(*$2!="main") yyerror("Error : Main not recognized");
								$$ = $5;
								
							}

PROCEDURE				: LOCAL_DECLARATIONS //STATEMENT_LIST
							{
								$$ = new Procedure(curr_proc_type, curr_proc_name, yylineno);
								$$->set_local_list(*$1);
							}

LOCAL_DECLARATIONS		: /* epsilon */
							{
								$$ = new Symbol_Table();
							}
						| LOCAL_DECLARATIONS DECLARATION
							{
								$1->append_list(*$2,yylineno);
								$$ = $1;
								$$->set_table_scope(local);
							}


// STATEMENT_LIST			: /* epsilon */
// 							{

// 							}
//  						| STATEMENT_LIST STATEMENT
//  							{

//  							}

// STATEMENT				: NAME '=' EXPRESSION  ';' 
// 							{printf("found a statement\n");}

// EXPRESSION 				: INTEGER_NUMBER
// 							{printf("found an expression consisting of a number\n");}
// 						| NAME
// 							{printf("found an expression consisting of a identifier\n");}
// 						| DOUBLE_NUMBER
// 							{printf("found an expression consisting of a number\n");}		
// 						| EXPRESSION  '+' EXPRESSION 
// 							{ printf("found a PLUS expression\n");}
// 						| EXPRESSION  '*' EXPRESSION 
// 							{ printf("found a MULT expression\n");}
// 						| EXPRESSION  '-' EXPRESSION 
// 							{ printf("found a SUB expression\n");}
// 						| EXPRESSION  '/' EXPRESSION 
// 							{ printf("found a DIV expression\n");}

 
%%

extern YYSTYPE yylval;