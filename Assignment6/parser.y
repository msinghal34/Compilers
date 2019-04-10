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

Table_Scope curr_table_scope;

Symbol_Table* global_symbol_table;
Symbol_Table* local_symbol_table;

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
	Relational_Op relop_value; 
	Sequence_Ast * seq_ast;
};
%token <string_value> INTEGER FLOAT VOID NAME IF ELSE DO WHILE AND OR NOT FOR PRINT
%token <double_value> DOUBLE_NUMBER
%token <integer_value> INTEGER_NUMBER 
%token <relop_value> RELOP EQOP
%right '=' ':' '?'
%left  OR
%left  AND
%left  EQOP
%left  RELOP
%left '+' '-'
%left '*' '/'
%start PROGRAM 
%nterm <symbol_table> GLOBAL_DECLARATIONS LOCAL_DECLARATIONS
%nterm <symbol_table> DECLARATION
%nterm <symbol_table> NAME_LIST
%nterm <string_value> TYPE
%nterm <procedure> MAIN_FUNCTION PROCEDURE
%nterm <ast_list> STATEMENT_LIST 
%nterm <ast> STATEMENT ASSIGN_STATEMENT ARITH_EXP COND_EXP IF_STATEMENT PRINT_STATEMENT
%nterm <ast> DO_WHILE_STATEMENT WHILE_STATEMENT IF_ELSE_STATEMENT BALANCED_IF_STATEMENT
%nterm <ast>  ASSIGN_STATEMENT_VERIFIED COND_EXP_VERIFIED
%nterm <seq_ast> SEQUENCE_STATEMENT_LIST  
%%

PROGRAM					: GLOBAL_DECLARATIONS MAIN_FUNCTION
							{
								program_object.set_global_table(*$1);
								program_object.set_procedure($2,yylineno);
							}

GLOBAL_DECLARATIONS		: /* epsilon */
							{
								$$ = new Symbol_Table();
								global_symbol_table = $$;
								curr_table_scope = global;
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
								Symbol_Table_Entry *ste = new Symbol_Table_Entry(*$1,curr_data_type,yylineno);
								ste->set_symbol_scope(curr_table_scope);
								$$->push_symbol(ste);
							}
						| NAME_LIST ',' NAME
							{
								Symbol_Table_Entry *ste = new Symbol_Table_Entry(*$3,curr_data_type,yylineno);
								ste->set_symbol_scope(curr_table_scope);
								$1->push_symbol(ste);
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
								$$ = $5;	
							}

PROCEDURE				: LOCAL_DECLARATIONS STATEMENT_LIST
							{
								$$ = new Procedure(curr_proc_type, curr_proc_name, yylineno);
								$$->set_local_list(*$1);
								$$->set_ast_list(*$2);
							}

LOCAL_DECLARATIONS		: /* epsilon */
							{
								$$ = new Symbol_Table();
								local_symbol_table = $$;
								curr_table_scope = local;
							}
						| LOCAL_DECLARATIONS DECLARATION
							{
								$1->append_list(*$2,yylineno);
								$$ = $1;
								$$->set_table_scope(curr_table_scope);
							}


STATEMENT_LIST			: /* epsilon */
							{
								$$ = new list<Ast*>();
							}
 						| STATEMENT_LIST STATEMENT
 							{
 								$1->push_back($2);
 								$$ = $1;
 							}
 						| STATEMENT_LIST '{' SEQUENCE_STATEMENT_LIST '}'
 						{
 							$1->push_back($3);
 							$$ = $1;
 						}

STATEMENT 				: ASSIGN_STATEMENT_VERIFIED
						| IF_STATEMENT
						| IF_ELSE_STATEMENT
						| WHILE_STATEMENT
						| DO_WHILE_STATEMENT
						| PRINT_STATEMENT

PRINT_STATEMENT 		: PRINT NAME ';'
						{
							Symbol_Table_Entry *v;
							//cout<<*$1<<"\n";
							if (local_symbol_table->variable_in_symbol_list_check(*$2))
							{
								v = &(local_symbol_table->get_symbol_table_entry(*$2));
							}
							else if (global_symbol_table->variable_in_symbol_list_check(*$2))
							{
								v = &(global_symbol_table->get_symbol_table_entry(*$2));
							}
							else
							{
								printf("\ncs316: Error %d,  Name Error \n", yylineno);
								exit(0);
							}
							Name_Ast *name = new Name_Ast(*$2, *v, yylineno);
							name->set_data_type(v->get_data_type());
							$$ = new Print_Ast(name, yylineno);
						}
DO_WHILE_STATEMENT		: DO '{' SEQUENCE_STATEMENT_LIST '}' WHILE '(' COND_EXP_VERIFIED ')' ';'
						{
							$$ = new Iteration_Statement_Ast($7,$3,yylineno,true);
						}
						|
						DO STATEMENT WHILE '(' COND_EXP_VERIFIED ')' ';'
						{
							$$ = new Iteration_Statement_Ast($5,$2,yylineno,true);
						}

WHILE_STATEMENT			: WHILE '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Iteration_Statement_Ast($3,$6,yylineno,false);
						}
						| WHILE '(' COND_EXP_VERIFIED ')' STATEMENT
						{
							$$ = new Iteration_Statement_Ast($3,$5,yylineno,false);
						}

IF_STATEMENT			: IF '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Selection_Statement_Ast($3,$6,NULL,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' STATEMENT
						{
							$$ = new Selection_Statement_Ast($3,$5,NULL,yylineno);
						}

BALANCED_IF_STATEMENT 	: ASSIGN_STATEMENT_VERIFIED
						{
							$$ = $1;
						}
						| DO_WHILE_STATEMENT
						{
							$$ = $1;
						}
						| WHILE '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Iteration_Statement_Ast($3,$6,yylineno,false);
						}
						| WHILE '(' COND_EXP_VERIFIED ')' BALANCED_IF_STATEMENT
						{
							$$ = new Iteration_Statement_Ast($3,$5,yylineno,false);
						}
						| PRINT_STATEMENT
						{
							$$ = $1;
						}
						| IF '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}' ELSE '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Selection_Statement_Ast($3,$6,$10,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' BALANCED_IF_STATEMENT ELSE '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Selection_Statement_Ast($3,$5,$8,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' BALANCED_IF_STATEMENT ELSE BALANCED_IF_STATEMENT
						{
							$$ = new Selection_Statement_Ast($3,$5,$7,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}' ELSE BALANCED_IF_STATEMENT
						{
							$$ = new Selection_Statement_Ast($3,$6,$9,yylineno);
						}

IF_ELSE_STATEMENT 		: IF '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}' ELSE '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Selection_Statement_Ast($3,$6,$10,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' BALANCED_IF_STATEMENT ELSE '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$$ = new Selection_Statement_Ast($3,$5,$8,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' BALANCED_IF_STATEMENT ELSE STATEMENT
						{
							$$ = new Selection_Statement_Ast($3,$5,$7,yylineno);
						}
						| IF '(' COND_EXP_VERIFIED ')' '{' SEQUENCE_STATEMENT_LIST '}' ELSE STATEMENT
						{
							$$ = new Selection_Statement_Ast($3,$6,$9,yylineno);
						}

SEQUENCE_STATEMENT_LIST	:/* epsilon */
						{
							$$ = new Sequence_Ast(yylineno);
						}
						|
						 SEQUENCE_STATEMENT_LIST STATEMENT
						{
							$1->ast_push_back($2);
							$$=$1;
						}
						| SEQUENCE_STATEMENT_LIST '{' SEQUENCE_STATEMENT_LIST '}'
						{
							$1->ast_push_back($3);
							$$ = $1;
						}

ASSIGN_STATEMENT_VERIFIED: ASSIGN_STATEMENT
						{
							if(!$1->check_ast())
							{
								printf("\ncs316: Error %d,  Assignment Statement data type not compatible \n", yylineno);
								exit(0);
							}
							$$ = $1;
						} 

COND_EXP_VERIFIED		: COND_EXP
						{
							if(!$1->check_ast())
							{
								printf("\ncs316: Error %d,  Conditional Expression Statement data type not compatible \n", yylineno);
								exit(0);
							}
							$$ = $1;

						}

ASSIGN_STATEMENT		: NAME '=' ARITH_EXP  ';' 
							{
								Symbol_Table_Entry *v;
								if (local_symbol_table->variable_in_symbol_list_check(*$1))
								{
									v = &(local_symbol_table->get_symbol_table_entry(*$1));
								}
								else if (global_symbol_table->variable_in_symbol_list_check(*$1))
								{
									v = &(global_symbol_table->get_symbol_table_entry(*$1));
								}
								else
								{
									printf("\ncs316: Error %d,  Name Error \n", yylineno);
									exit(0);
								}
								Name_Ast * name_ast = new Name_Ast(*$1, *v, yylineno);
								if (name_ast->get_data_type() == $3->get_data_type())
								{
									$$ = new Assignment_Ast(name_ast, $3, yylineno);								
								}
								else
								{
									printf("\ncs316: Error %d,  Assignment statement data type not compatible \n", yylineno);
									exit(0);
								}
							}

COND_EXP				: ARITH_EXP RELOP ARITH_EXP
						{
							$$ = new Relational_Expr_Ast($1, $2, $3, yylineno);
							$$->set_data_type($1->get_data_type());
						}
						| ARITH_EXP EQOP ARITH_EXP
						{
							$$ = new Relational_Expr_Ast($1, $2, $3, yylineno);
							$$->set_data_type($1->get_data_type());
						}
						| COND_EXP AND COND_EXP
						{
							$$ = new Logical_Expr_Ast($1, _logical_and, $3, yylineno);
							$$->set_data_type(int_data_type);
						}
						| COND_EXP OR COND_EXP
						{
							$$ = new Logical_Expr_Ast($1, _logical_or, $3, yylineno);
							$$->set_data_type(int_data_type);
						}
						| NOT COND_EXP %prec '*'
						{
							$$ = new Logical_Expr_Ast(NULL, _logical_not, $2, yylineno);
							$$->set_data_type(int_data_type);
						}
						| COND_EXP '?' COND_EXP ':' COND_EXP
						{
							if(!$1->check_ast())
							{
								printf("\ncs316: Error %d,  Conditional Expression Statement data type not compatible \n", yylineno);
								exit(0);
							}
							$$ = new Conditional_Expression_Ast($1, $3, $5, yylineno);
							$$->set_data_type(int_data_type);
						}
						| '(' COND_EXP ')'
						{
							$$ = $2;
						}

ARITH_EXP 				: INTEGER_NUMBER
							{
								$$ = new Number_Ast<int>($1, int_data_type, yylineno);
								$$->set_data_type(int_data_type);
							}
						| NAME
							{
								Symbol_Table_Entry *v;
								if (local_symbol_table->variable_in_symbol_list_check(*$1))
								{
									v = &(local_symbol_table->get_symbol_table_entry(*$1));
								}
								else if (global_symbol_table->variable_in_symbol_list_check(*$1))
								{
									v = &(global_symbol_table->get_symbol_table_entry(*$1));
								}
								else
								{
									printf("\ncs316: Error %d,  Name Error \n", yylineno);
									exit(0);
								}
								$$ = new Name_Ast(*$1, *v, yylineno);
								$$->set_data_type(v->get_data_type()); 
							}
						| DOUBLE_NUMBER
							{
								$$ = new Number_Ast<double>($1, double_data_type, yylineno);
								$$->set_data_type(double_data_type);
							}		
						| ARITH_EXP '+' ARITH_EXP 
							{
								if ($1->get_data_type() == $3->get_data_type())
								{
									$$ = new Plus_Ast($1, $3, yylineno);
									$$->set_data_type($1->get_data_type());								
								}
								else
								{
									printf("\ncs316: Error %d,  Assignment statement data type not compatible \n", yylineno);
									exit(0);
								}
							}
						| ARITH_EXP  '*' ARITH_EXP 
							{
								if ($1->get_data_type() == $3->get_data_type())
								{
									$$ = new Mult_Ast($1, $3, yylineno);
									$$->set_data_type($1->get_data_type());								
								}
								else
								{
									printf("\ncs316: Error %d,  Assignment statement data type not compatible \n", yylineno);
									exit(0);
								}

							}
						| ARITH_EXP  '-' ARITH_EXP 
							{
								if ($1->get_data_type() == $3->get_data_type())
								{
									$$ = new Minus_Ast($1, $3, yylineno);
									$$->set_data_type($1->get_data_type());								
								}
								else
								{
									printf("\ncs316: Error %d,  Assignment statement data type not compatible \n", yylineno);
									exit(0);
								}

							}
						| ARITH_EXP  '/' ARITH_EXP 
							{
								if ($1->get_data_type() == $3->get_data_type())
								{
									$$ = new Divide_Ast($1, $3, yylineno);
									$$->set_data_type($1->get_data_type());								
								}
								else
								{
									printf("\ncs316: Error %d,  Assignment statement data type not compatible \n", yylineno);
									exit(0);
								}
							}
						| '-'  ARITH_EXP %prec '*'
							{
								$$ = new UMinus_Ast($2, NULL, yylineno);
								$$->set_data_type($2->get_data_type());
							}
						| '(' ARITH_EXP ')'
							{
								$$ = $2;
							}
						| COND_EXP '?' ARITH_EXP ':' ARITH_EXP 
							{
								if(!$1->check_ast())
								{
									printf("\ncs316: Error %d,  Conditional Expression Statement data type not compatible \n", yylineno);
									exit(0);
								}
								$$ = new Conditional_Expression_Ast($1, $3, $5, yylineno);
								$$->set_data_type($3->get_data_type());
							}
%%

extern YYSTYPE yylval;