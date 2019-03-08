
#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include<string>
#include<map>
#include<list>

#define PROC_SPACE "   "
#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class Procedure
{
	Data_Type return_type;

	string name;

	Symbol_Table local_symbol_table;

	Basic_Block * basic_block;

	list<Ast *> statement_list;

	list<Icode_Stmt *> bb_icode_list;

	int lineno;

public:
	Procedure(Data_Type proc_return_type, string proc_name, int line);

	~Procedure();

	string get_proc_name();

	void set_basic_block(Basic_Block & bb_list);
	
	void set_ast_list(list<Ast *> & ast_list);

	void set_local_list(Symbol_Table & new_list);
	
    Symbol_Table get_local_list();

	Data_Type get_return_type();
	
	Symbol_Table_Entry & get_symbol_table_entry(string variable_name);

	void print(ostream & file_buffer);
	
	void print_sym(ostream & file_buffer);

	void evaluate_print(ostream & fil_buffer);
	
	Eval_Result & evaluate(ostream & file_buffer);

	bool variable_in_symbol_list_check(string variable);

	// compile
	void compile();
	void print_icode(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);

private:
	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif
