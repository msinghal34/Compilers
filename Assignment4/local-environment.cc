#include "local-environment.hh"
////////////////////////////////////////////////////////////
int Eval_Result::get_int_value()
{
	return 0;
}
void Eval_Result::set_value(int value)
{
}
double Eval_Result::get_double_value()
{
	return 0.0;
}
void Eval_Result::set_value(double value)
{
}
bool Eval_Result::is_variable_defined()
{
	return false;
}
void Eval_Result::set_variable_status(bool def)
{
}
////////////////////////////////////////////////////////////
int Eval_Result_Value::get_int_value()
{
	return 0;
}
void Eval_Result_Value::set_value(int value)
{
}
double Eval_Result_Value::get_double_value()
{
	return 0.0;
}
void Eval_Result_Value::set_value(double value)
{
}
////////////////////////////////////////////////////////////
Eval_Result_Value_Int::Eval_Result_Value_Int()
{
}
Eval_Result_Value_Int::~Eval_Result_Value_Int()
{
}
void Eval_Result_Value_Int::set_value(int number)
{
	value = number;
}
void Eval_Result_Value_Int::set_value(double number)
{
	value = number;
}
int Eval_Result_Value_Int::get_int_value()
{
	return value;
}
void Eval_Result_Value_Int::set_variable_status(bool def)
{
	defined = def;
}
bool Eval_Result_Value_Int::is_variable_defined()
{
	return defined;
}
void Eval_Result_Value_Int::set_result_enum(Result_Enum res)
{
	result_type = res;
}
Result_Enum Eval_Result_Value_Int::get_result_enum()
{
	return result_type;
}
////////////////////////////////////////////////////////////
Eval_Result_Value_Double::Eval_Result_Value_Double()
{
}
Eval_Result_Value_Double::~Eval_Result_Value_Double()
{
}
void Eval_Result_Value_Double::set_value(int number)
{
	value = number;
}
void Eval_Result_Value_Double::set_value(double number)
{
	value = number;
}
double Eval_Result_Value_Double::get_double_value()
{
	return value;
}
void Eval_Result_Value_Double::set_variable_status(bool def)
{
	defined = def;
}
bool Eval_Result_Value_Double::is_variable_defined()
{
	return defined;
}
void Eval_Result_Value_Double::set_result_enum(Result_Enum res)
{
	result_type = res;
}
Result_Enum Eval_Result_Value_Double::get_result_enum()
{
	return result_type;
}
////////////////////////////////////////////////////////////
Local_Environment::Local_Environment()
{
}
Local_Environment::~Local_Environment()
{
}
void Local_Environment::print(ostream &file_buffer)
{
	//TODO Complete this
}
bool Local_Environment::is_variable_defined(string name)
{
	if (variable_table.count(name) > 0)
	{
		return variable_table[name]->is_variable_defined();
	}
	else
	{
		return interpreter_global_table.is_variable_defined(name);
	}
}
Eval_Result *Local_Environment::get_variable_value(string name)
{
	if (variable_table.count(name) > 0)
	{
		return variable_table[name];
	}
	else
	{
		return interpreter_global_table.get_variable_value(name);
	}
}
void Local_Environment::put_variable_value(Eval_Result &value, string name)
{
	if (variable_table.count(name) > 0)
	{
		variable_table[name] = &value;
	}
	else
	{
		interpreter_global_table.put_variable_value(value, name);
	}
}
bool Local_Environment::does_variable_exist(string name)
{
	return true;
}
////////////////////////////////////////////////////////////
void Symbol_Table::create(Local_Environment &local_global_variables_table)
{
	list<Symbol_Table_Entry *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); ++it)
	{
		string name = (*it)->get_variable_name();
		Data_Type dt = (*it)->get_data_type();
		if (dt == int_data_type)
		{
			Eval_Result_Value_Int *evalResult = new Eval_Result_Value_Int();
			evalResult->set_variable_status(false);
			local_global_variables_table.put_variable_value(*evalResult, name);
		}
		else
		{
			Eval_Result_Value_Double *evalResult = new Eval_Result_Value_Double();
			evalResult->set_variable_status(false);
			local_global_variables_table.put_variable_value(*evalResult, name);
		}
	}
}
////////////////////////////////////////////////////////////