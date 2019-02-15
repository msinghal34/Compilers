
template class Number_Ast<double>;
template class Number_Ast<int>;

Data_Type Ast::get_data_type(){
	return node_data_type;
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
	if(constant==0) return true;
	else return false;
}





void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}





