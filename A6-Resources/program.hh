

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
	map<string, Procedure *> procedure_map;

public:
	Program();
	~Program();
	void delete_all();
	Procedure * get_procedure_prototype(string proc_name);
	void set_proc_to_map(string proc_name, Procedure * proc);

	void called_proc_are_defined_check();

	void set_procedure_map(map<string, Procedure *> & proc);

	void set_procedure(Procedure * proc, int line);
	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	void print_sym();
	void print();


	Procedure * get_main_procedure(ostream & file_buffer);
	void evaluate_print();
	Eval_Result & evaluate();

	bool variable_proc_name_check(string symbol);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_check();

    bool variable_in_proc_map_check(string symbol);
	void global_list_in_proc_map_check();

    bool is_procedure_exists(string proc_name);

	// compile
	void compile();
	void print_assembly();
};

#endif
