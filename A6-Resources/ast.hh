

#ifndef AST_HH
#define AST_HH

#include<string>
#include<iostream>
#include<iomanip>
#include<typeinfo>
#include<list>

#define AST_SPACE "         "
#define AST_NODE_SPACE "            "
#define AST_SUB_NODE_SPACE "               "

#define END_OF_PROG -3	
// Used by goto_ast and basic_block

using namespace std;

class Ast;

class Ast
{
protected:
	typedef enum
	{
		zero_arity = 0,
		unary_arity = 1,
		binary_arity = 2,
		ternary_arity = 3
	}Ast_Arity;

	Data_Type node_data_type;
	Ast_Arity ast_num_child;

   static int labelCounter;
	int lineno;
	string get_new_label(){

		return "label"+to_string(labelCounter++);
	}

public:
	Ast();
	~Ast();

	virtual Data_Type get_data_type();
	virtual void set_data_type(Data_Type dt);

	virtual bool is_value_zero();

	virtual bool check_ast();
	virtual Symbol_Table_Entry & get_symbol_entry();

	virtual void print(ostream & file_buffer) = 0;
	virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

	virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;

	virtual Code_For_Ast & compile() = 0;
	virtual Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra) = 0;
	virtual Code_For_Ast & create_store_stmt(Register_Descriptor * store_register);
};

class Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

public:
	Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line);
	~Assignment_Ast();

	bool check_ast();

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Name_Ast:public Ast
{
	Symbol_Table_Entry * variable_symbol_entry;

public:
	Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line);
	~Name_Ast();

	Data_Type get_data_type();
	Symbol_Table_Entry & get_symbol_entry();
	void set_data_type(Data_Type dt);

	void print(ostream & file_buffer);

	void print_value(Local_Environment & eval_env, ostream & file_buffer);
	Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
	void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
	Code_For_Ast & create_store_stmt(Register_Descriptor * store_register);
};

template <class T>
class Number_Ast:public Ast
{
	T constant;

public:
	Number_Ast(T number, Data_Type constant_data_type, int line);
	~Number_Ast();

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);
	bool is_value_zero();

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Arithmetic_Expr_Ast:public Ast
{
protected:
	Ast * lhs;
	Ast * rhs;

public:
	Arithmetic_Expr_Ast() {}
	~Arithmetic_Expr_Ast() {}

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);
	bool check_ast();

	virtual void print(ostream & file_buffer) = 0;
	
	virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;

	// compile
	virtual Code_For_Ast & compile() = 0;
	virtual Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra) = 0;
};

class Plus_Ast:public Arithmetic_Expr_Ast
{
public:
	Plus_Ast(Ast * l, Ast * r, int line);
	~Plus_Ast() {}

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Minus_Ast:public Arithmetic_Expr_Ast
{
public:
	Minus_Ast(Ast * l, Ast * r, int line);
	~Minus_Ast() {}

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Divide_Ast:public Arithmetic_Expr_Ast
{
public:
	Divide_Ast(Ast * l, Ast * r, int line);
	~Divide_Ast() {}

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Mult_Ast:public Arithmetic_Expr_Ast
{
public:
	Mult_Ast(Ast * l, Ast * r, int line);
	~Mult_Ast() {}

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class UMinus_Ast:public Arithmetic_Expr_Ast
{
public:
	UMinus_Ast(Ast * l, Ast * r, int line);
	~UMinus_Ast() {}
	
	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};

class Conditional_Expression_Ast: public Arithmetic_Expr_Ast

{
protected:
	Ast* cond;
public:
	Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line);
	~Conditional_Expression_Ast();

	void print(ostream & file_buffer);

	Code_For_Ast & compile();
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};


class Print_Ast:public Ast
{
     Ast *var;
public:
	Print_Ast(Ast *v, int line);
	~Print_Ast();

	void print(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer){}

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};

class Relational_Expr_Ast:public Ast
{
	Ast * lhs_condition;
	Ast * rhs_condition;
	Relational_Op rel_op;

public:
	Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line);
	~Relational_Expr_Ast();

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);

	bool check_ast();

	void print(ostream & file_buffer);
	Code_For_Ast & compile();

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};

class Logical_Expr_Ast:public Ast
{
	Ast * lhs_op;
	Ast * rhs_op;
	Logical_Op bool_op;

public:
	Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line);
	~Logical_Expr_Ast();

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);

	bool check_ast();

	void print(ostream & file_buffer);

	Code_For_Ast & compile();
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};

class Selection_Statement_Ast: public Ast {
protected:
	Ast* cond;
	Ast* then_part;
	Ast* else_part;
public:
	Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line);
	~Selection_Statement_Ast();

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);

	bool check_ast();

	void print(ostream & file_buffer);

	Code_For_Ast & compile();
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};

class Iteration_Statement_Ast: public Ast {
protected:
	Ast* cond;
	Ast* body;
	bool is_do_form;
public:
	Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form);
	~Iteration_Statement_Ast();

	Data_Type get_data_type();
	void set_data_type(Data_Type dt);

	bool check_ast();

	void print(ostream & file_buffer);

	Code_For_Ast & compile();
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};



class Sequence_Ast: public Ast{
	list<Ast *> statement_list;
	list<Icode_Stmt *> sa_icode_list;
public:
	Sequence_Ast(int line);
	~Sequence_Ast();
  void ast_push_back(Ast * ast);
	void print(ostream & file_buffer);
	Code_For_Ast & compile();
	void print_assembly(ostream & file_buffer);
	void print_icode(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra){}
};

class Call_Ast:public Ast
{
	string procedure_name;
	list<Ast *> actual_param_list;

	Register_Descriptor * return_value_reg;

public:
	Call_Ast(string name, int line);
	~Call_Ast();

	Data_Type get_data_type();
	void set_register(Register_Descriptor * reg);

	void check_actual_formal_param(Symbol_Table & formal_param_list);
	void set_actual_param_list(list<Ast *> & param_list);

	void print(ostream & file_buffer);
	
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};	

class Return_Ast:public Ast
{
	Ast * return_value;
	string proc_name;
public:
	Return_Ast(Ast * ret_val, string name, int line);
	~Return_Ast();

	Data_Type get_data_type();

	void print(ostream & file_buffer);
	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	Code_For_Ast & compile();
	Code_For_Ast & compile_and_optimize_ast(Lra_Outcome & lra);
};




#endif
