

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

	Symbol_Table formal_param_table;

	Basic_Block * basic_block;

	list<Ast *> statement_list;

	list<Icode_Stmt *> bb_icode_list;

	int lineno;


	bool proc_is_called;
	bool proc_is_defined;

public:
	Procedure(Data_Type proc_return_type, string proc_name, int line);
	~Procedure();

	void operator==(Procedure & proc_proto);
	void operator=(Procedure & proc_proto);

	bool is_proc_called();
	void set_proc_is_called();
	bool is_proc_defined();
	void set_proc_is_defined();

	string get_proc_name();
	void set_basic_block_list(list<Basic_Block *> & bb_list);
	void set_local_list(Symbol_Table & new_list);
	Symbol_Table get_local_list();

	void set_ast_list(list<Ast *> & ast_list);
	
	void set_formal_param_list(Symbol_Table & new_list);
	Symbol_Table & get_formal_param_list();
	Data_Type get_return_type();
	void set_return_type(Data_Type ret_type);
	Symbol_Table_Entry & get_symbol_table_entry(string variable_name);
	Symbol_Table_Entry & get_formal_param_entry(string variable_name);

	void print(ostream & file_buffer);
	void print_sym(ostream & file_buffer);

	Basic_Block * get_bb(int line);
	Basic_Block * get_next_bb(Basic_Block & current_bb);
	Basic_Block & get_start_basic_block();
	void evaluate_print(ostream & fil_buffer);
	Eval_Result & evaluate(Local_Environment eval_env, ostream & file_buffer);

	bool variable_in_symbol_list_check(string variable);
	bool variable_in_formal_list_check(string variable);
	bool basic_block_in_list_check(int bb_number);

	// compile
	string get_variable_in_formal_list(int position);

	void compile();
	void print_icode(ostream & file_buffer);
	void print_assembly(ostream & file_buffer);

private:
	void print_prologue(ostream & file_buffer);
	void print_epilogue(ostream & file_buffer);
};

#endif
