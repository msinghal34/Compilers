#include <stdlib.h>
template class Number_Ast<double>;
template class Number_Ast<int>;

///////////////////////////////////////////////////////////////////
Ast::Ast(){

}

Ast::~Ast(){

}

Data_Type Ast::get_data_type(){
	return node_data_type;
}

void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Ast::is_value_zero(){
	return true;
}

bool Ast::check_ast(){
	return true;
}

///////////////////////////////////////////////////////////////////
Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
	lhs = temp_lhs;
	rhs = temp_rhs;
	lineno = line;
}

Assignment_Ast::~Assignment_Ast(){
	free(lhs);
	free(rhs);
}

bool Assignment_Ast::check_ast(){
	return ((lhs->get_data_type() == rhs->get_data_type()) && lhs->check_ast() && rhs->check_ast());
}

void Assignment_Ast::print(ostream & file_buffer){
	// SEE LATER
	file_buffer<<"Hi"<<endl;
}

///////////////////////////////////////////////////////////////////
Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	variable_symbol_entry = &var_entry;
	lineno = line;
	// SEE LATER for name
}

Name_Ast::~Name_Ast(){
	free(variable_symbol_entry);
}

Data_Type Name_Ast::get_data_type(){
	return node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer){
	// SEE LATER
	file_buffer<<"Hi"<<endl;
}

///////////////////////////////////////////////////////////////////
template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
	constant = number;
	node_data_type = constant_data_type;
	lineno = line;
}

template <class T>
Number_Ast<T>::~Number_Ast(){

}

template <class T>
Data_Type Number_Ast<T>::get_data_type(){
	return node_data_type;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
	node_data_type = dt;
}

template <class T>
bool Number_Ast<T>::is_value_zero(){
	if (constant == 0) return true;
	else return false;
}

template <class T>
void Number_Ast<T>::print(ostream & file_buffer){
	//SEE LATER
	file_buffer<<constant<<endl;
}

// ///////////////////////////////////////////////////////////////////
// Arithmetic_Expr_Ast::Arithmetic_Expr_Ast(){

// }

// Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast(){
// 	free(lhs);
// 	free(rhs);
// }

// Data_Type Arithmetic_Expr_Ast::get_data_type(){
// 	return node_data_type;
// }

// Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
// 	node_data_type = dt;
// }

// bool Arithmetic_Expr_Ast::check_ast(){
// 	return ((lhs->get_data_type() == rhs->get_data_type()) && lhs->check_ast() && rhs->check_ast()); 
// }

// void Arithmetic_Expr_Ast::print(ostream & file_buffer){
// 	//SEE LATER
// 	file_buffer<<"Hi"<<endl;
// }

// ///////////////////////////////////////////////////////////////////
Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
	lineno = line;
	lhs = l;
	rhs = r;
}

// Plus_Ast::~Plus_Ast(){
// 	free(lhs);
// 	free(rhs);
// }

// void Plus_Ast::print(ostream & file_buffer){
// 	//SEE LATER
// 	file_buffer<<"Hi"<<endl;
// }

// ///////////////////////////////////////////////////////////////////
Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
	lineno = line;
	lhs = l;
	rhs = r;
}

// Minus_Ast::~Minus_Ast(){
// 	free(lhs);
// 	free(rhs);
// }

// void Minus_Ast::print(ostream & file_buffer){
// 	//SEE LATER
// 	file_buffer<<"Hi"<<endl;
// }

// ///////////////////////////////////////////////////////////////////
Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
	lineno = line;
	lhs = l;
	rhs = r;
}

// Mult_Ast::~Mult_Ast(){
// 	free(lhs);
// 	free(rhs);
// }

// void Mult_Ast::print(ostream & file_buffer){
// 	//SEE LATER
// 	file_buffer<<"Hi"<<endl;
// }

// ///////////////////////////////////////////////////////////////////
Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
	lineno = line;
	lhs = l;
	rhs = r;
}

// Divide_Ast::~Divide_Ast(){
// 	free(lhs);
// 	free(rhs);
// }

// void Divide_Ast::print(ostream & file_buffer){
// 	//SEE LATER
// 	file_buffer<<"Hi"<<endl;
// }

// ///////////////////////////////////////////////////////////////////
UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	lineno = line;
	lhs = l;
	rhs = r;
}

// UMinus_Ast::~UMinus_Ast(){
// 	free(lhs);
// 	free(rhs);
// }

// void UMinus_Ast::print(ostream & file_buffer){
// 	//SEE LATER
// 	file_buffer<<"Hi"<<endl;
// }

// ///////////////////////////////////////////////////////////////////