
#include<string>
#include<fstream>

using namespace std;



Local_Environment interpreter_global_table;

Eval_Result & Program::evaluate()
{
	Procedure * main_proc = procedure;
	CHECK_INPUT_AND_ABORT(((main_proc != NULL) && (main_proc->get_proc_name() == "main")), 
		"No main function found in the program", NO_FILE_LINE);

	global_symbol_table.create(interpreter_global_table);

	command_options.create_output_buffer();
	ostream & file_buffer = command_options.get_output_buffer();
	file_buffer << "Evaluating Program\n";
	file_buffer << GLOB_SPACE << "Global Variables (before evaluating):\n";

	interpreter_global_table.print(file_buffer);

	Eval_Result & result = main_proc->evaluate(file_buffer);

	file_buffer << GLOB_SPACE << "Global Variables (after evaluating):\n";
	interpreter_global_table.print(file_buffer);

	return result;
}
