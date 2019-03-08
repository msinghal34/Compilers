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
	map<string, Eval_Result *>::iterator it;
	for (it = variable_table.begin(); it != variable_table.end(); ++it)
	{
		string name = (*it).first;
		Eval_Result *evalResult = (*it).second;
		if (name[0] != '1')
		{	
			Result_Enum resultEnum = evalResult->get_result_enum();
			if (resultEnum == int_result)
			{
				if (evalResult->is_variable_defined())
				{
					int value = evalResult->get_int_value();
					file_buffer<<AST_SPACE<<name<<" : "<<value<<endl;
				}
				else
				{
					file_buffer<<AST_SPACE<<name<<" : "<<"undefined"<<endl;	
				}
			}
			else
			{
				if (evalResult->is_variable_defined())
				{
					double value = evalResult->get_double_value();
					file_buffer<<AST_SPACE<<name<<" : "<<value<<endl;
				}
				else
				{
					file_buffer<<AST_SPACE<<name<<" : "<<"undefined"<<endl;	
				}
			}
		}
	}
}
bool Local_Environment::is_variable_defined(string name)
{
	if (variable_table.count("1local") > 0)
	{
		if (variable_table.count(name) > 0)
		{
			return true;
		}
		else 
		{
			return interpreter_global_table.is_variable_defined(name);
		}
	}
	else if (variable_table.count("1global") > 0)
	{
		if (variable_table.count(name) > 0)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
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
	if (name[0] == '1')
	{
		// cout<<"ok7"<<endl;
		// cout<<name<<endl;
		variable_table[name] = &value;
		if (interpreter_global_table.is_variable_defined("1global") == false)
		{
			Eval_Result_Value_Int *evalResult = new Eval_Result_Value_Int();
			evalResult->set_variable_status(false);
			evalResult->set_result_enum(int_result);
			interpreter_global_table.put_variable_value(*evalResult, "1global");
		}
		// cout<<"meow"<<endl;
		// put_variable_value(value, name);
		// cout<<"ok8"<<endl;
	}
	else
	{
		// cout<<"hi"<<endl;
		// cout<<"here\n";
		if (variable_table.count("1local") > 0)
		{
			// cout<<name<<endl;
			// It means I am in local table
			if (variable_table.count(name) > 0)
			{
				variable_table[name] = &value;
			}
			else if (interpreter_global_table.is_variable_defined(name) > 0)
			{
				cout<<"What"<<endl;
				interpreter_global_table.put_variable_value(value, name);
			}
			else
			{
				// cout<<"Hmm"<<endl;
				variable_table[name] = &value;
				// put_variable_value(value, name);	
			}
		}	
		else if (variable_table.count("1global") > 0)
		{
			// It means I am in global table
			// put_variable_value(value, name);
			variable_table[name] = &value;
			// cout<<"yes"<<endl;
		}
		else
		{
			Eval_Result_Value_Int *evalResult = new Eval_Result_Value_Int();
			evalResult->set_variable_status(false);
			evalResult->set_result_enum(int_result);
			variable_table["1local"] = evalResult;
			put_variable_value(value, name);
			// interpreter_global_table.put_variable_value(*evalResult, "1global");
			// cout<<"Something is wrong"<<endl;
		}
	}
}
bool Local_Environment::does_variable_exist(string name)
{
	return true;
}
////////////////////////////////////////////////////////////
void Symbol_Table::create(Local_Environment &local_global_variables_table)
{
	// cout<<"Hello Brother"<<endl;
	list<Symbol_Table_Entry *>::iterator it;
	if (variable_table.begin() != variable_table.end())
	{
		Table_Scope ts = (*variable_table.begin())->get_symbol_scope();
		string name;
		if (ts == 0)
		{
			name = "1global";
		}
		else if (ts == 1)
		{
			name = "1local";
		}
		Eval_Result_Value_Int *evalResult = new Eval_Result_Value_Int();
		evalResult->set_variable_status(false);
		evalResult->set_result_enum(int_result);
		// cout<<name<<endl;
		// cout<<"ok1"<<endl;
		local_global_variables_table.put_variable_value(*evalResult, name);
		// cout<<"ok2"<<endl;
	}
	for (it = variable_table.begin(); it != variable_table.end(); ++it)
	{
		string name = (*it)->get_variable_name();
		Data_Type dt = (*it)->get_data_type();
		// Table_Scope ts = (*variable_table.begin())->get_symbol_scope();
		if (dt == int_data_type)
		{
			Eval_Result_Value_Int *evalResult = new Eval_Result_Value_Int();
			evalResult->set_variable_status(false);
			evalResult->set_result_enum(int_result);
			// cout<<"okk"<<endl;
			local_global_variables_table.put_variable_value(*evalResult, name);
			// cout<<"aokk"<<endl;
		}
		else
		{
			Eval_Result_Value_Double *evalResult = new Eval_Result_Value_Double();
			evalResult->set_result_enum(double_result);
			evalResult->set_variable_status(false);
			local_global_variables_table.put_variable_value(*evalResult, name);
		}
	}
}
////////////////////////////////////////////////////////////