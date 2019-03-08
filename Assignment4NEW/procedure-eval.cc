
#include<string>
#include<fstream>
#include<iostream>

using namespace std;


Eval_Result & Procedure::evaluate(ostream & file_buffer)
{
	Local_Environment & eval_env = *new Local_Environment();
	local_symbol_table.create(eval_env);
	
	Eval_Result * result = NULL;

	file_buffer << PROC_SPACE << "Evaluating Procedure " << name << "\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (before evaluating):\n";
	eval_env.print(file_buffer);
	file_buffer << "\n";
	

	//file_buffer << "\n" << BB_SPACE << "Basic Block: " << id_number << "\n";

	list <Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
	{
		CHECK_INVARIANT(((*i) != NULL), "Ast pointer seems to be NULL into the basic block");
		result = &((*i)->evaluate(eval_env, file_buffer)); 
	}


	//result = &(basic_block->evaluate(eval_env, file_buffer));

	file_buffer << "\n\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating):\n";
	eval_env.print(file_buffer);

	return *result;
}
