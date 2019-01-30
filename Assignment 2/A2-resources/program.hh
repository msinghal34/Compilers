
#ifndef PROGRAM_HH
#define PROGRAM_HH

#include<string>

#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;

class Program
{
	Symbol_Table global_symbol_table;
	Procedure * procedure;

public:
	Program();
	~Program();
	void delete_all();

	void set_procedure(Procedure * proc, int line);
	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	void print_sym();
	void print();

	void evaluate_print();
	Eval_Result & evaluate();

	bool variable_proc_name_check(string symbol);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_check();

	// compile
	void compile();
	void print_assembly();
};

#endif
