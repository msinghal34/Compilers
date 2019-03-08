// #include "ast.hh"
// #include "local-environment.hh"
// #include "symbol-table.hh"
template class Number_Ast<double>;
template class Number_Ast<int>;
////////////////////////////////////////////////////////////
Eval_Result &Ast::get_value_of_evaluation(Local_Environment &eval_env)
{
    Eval_Result &eval_result = *static_cast<Eval_Result *>(NULL);
    return eval_result;
}
void Ast::set_value_of_evaluation(Local_Environment &eval_env, Eval_Result &result)
{
}
void Ast::print_value(Local_Environment &eval_env, ostream &file_buffer)
{
}
////////////////////////////////////////////////////////////
Eval_Result &Assignment_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsResult = lhs->evaluate(eval_env, file_buffer);
    Eval_Result &rhsResult = rhs->evaluate(eval_env, file_buffer);
    // file_buffer<<"assn "<<rhsResult.get_result_enum()<<"\n";
    lhs->set_value_of_evaluation(eval_env, rhsResult);
    this->print(file_buffer);
    lhs->print_value(eval_env, file_buffer);
}
////////////////////////////////////////////////////////////
void Name_Ast::print_value(Local_Environment &eval_env, ostream &file_buffer)
{
    string name = variable_symbol_entry->get_variable_name();
    Result_Enum resultEnum ;
    int int_val;
    double double_val;
    if(eval_env.does_variable_exist(name)){
        Eval_Result *evalResult = eval_env.get_variable_value(name);
        resultEnum = evalResult->get_result_enum();
        int_val = evalResult->get_int_value();        
    }
    else{
        Eval_Result *evalResult = interpreter_global_table.get_variable_value(name);
        resultEnum = evalResult->get_result_enum();
        double_val = evalResult->get_int_value();
    } 
    if (resultEnum == int_result)
    {
        file_buffer <<"\n"<< AST_SPACE << name << " : " << int_val << "\n\n";
    }
    else if (resultEnum == double_result)
    {
        file_buffer <<"\n"<< AST_SPACE << name << " : " << double_val << "\n\n";
    }

   
}
Eval_Result &Name_Ast::get_value_of_evaluation(Local_Environment &eval_env)
{
    string name = variable_symbol_entry->get_variable_name();
    if(eval_env.does_variable_exist(name)){
        Eval_Result *evalResult = eval_env.get_variable_value(name);
        return *evalResult;
    }
    else{
        Eval_Result *evalResult = interpreter_global_table.get_variable_value(name);
        return *evalResult;
    }
}
void Name_Ast::set_value_of_evaluation(Local_Environment &eval_env, Eval_Result &result)
{
    string name = variable_symbol_entry->get_variable_name();
    if(eval_env.does_variable_exist(name)){
        eval_env.put_variable_value(result, name);
    }
    else{
        interpreter_global_table.put_variable_value(result, name);
    }    
}
Eval_Result &Name_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    string name = variable_symbol_entry->get_variable_name();
    if(eval_env.does_variable_exist(name)){
        Eval_Result *evalResult = eval_env.get_variable_value(name);
        return *evalResult;
    }
    else{
        Eval_Result *evalResult = interpreter_global_table.get_variable_value(name);
        return *evalResult;
    }
}
////////////////////////////////////////////////////////////
template <class T>
Eval_Result &Number_Ast<T>::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Data_Type dt = get_data_type();
    if (dt == int_data_type)
    {
        Eval_Result_Value_Int *num_eval = new Eval_Result_Value_Int();
        num_eval->set_value(constant);
        num_eval->set_result_enum(int_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
    else if (dt == double_data_type)
    {
        Eval_Result_Value_Double *num_eval = new Eval_Result_Value_Double();
        num_eval->set_value(constant);
        num_eval->set_result_enum(double_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
}
////////////////////////////////////////////////////////////

Eval_Result &Arithmetic_Expr_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &eval_result = *static_cast<Eval_Result *>(NULL);
    return eval_result;
}

////////////////////////////////////////////////////////////
Eval_Result &Plus_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsEval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result &rhsEval = rhs->evaluate(eval_env, file_buffer);
    Data_Type dt = get_data_type();
    if (dt == int_data_type)
    {
        Eval_Result_Value_Int *num_eval = new Eval_Result_Value_Int();
        num_eval->set_value(lhsEval.get_int_value() + rhsEval.get_int_value());
        num_eval->set_result_enum(int_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
    else if (dt == double_data_type)
    {
        Eval_Result_Value_Double *num_eval = new Eval_Result_Value_Double();
        num_eval->set_value(lhsEval.get_double_value() + rhsEval.get_double_value());
        num_eval->set_result_enum(double_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
}
////////////////////////////////////////////////////////////
Eval_Result &Minus_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsEval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result &rhsEval = rhs->evaluate(eval_env, file_buffer);
    Data_Type dt = get_data_type();
    if (dt == int_data_type)
    {
        Eval_Result_Value_Int *num_eval = new Eval_Result_Value_Int();
        num_eval->set_value(lhsEval.get_int_value() - rhsEval.get_int_value());
        num_eval->set_result_enum(int_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
    else if (dt == double_data_type)
    {
        Eval_Result_Value_Double *num_eval = new Eval_Result_Value_Double();
        num_eval->set_value(lhsEval.get_double_value() - rhsEval.get_double_value());
        num_eval->set_result_enum(double_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
}
////////////////////////////////////////////////////////////
Eval_Result &Mult_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsEval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result &rhsEval = rhs->evaluate(eval_env, file_buffer);
    Data_Type dt = get_data_type();
    if (dt == int_data_type)
    {
        Eval_Result_Value_Int *num_eval = new Eval_Result_Value_Int();
        num_eval->set_value(lhsEval.get_int_value() * rhsEval.get_int_value());
        num_eval->set_result_enum(int_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
    else if (dt == double_data_type)
    {
        Eval_Result_Value_Double *num_eval = new Eval_Result_Value_Double();
        num_eval->set_value(lhsEval.get_double_value() * rhsEval.get_double_value());
        num_eval->set_result_enum(double_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
}
////////////////////////////////////////////////////////////
Eval_Result &Divide_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsEval = lhs->evaluate(eval_env, file_buffer);
    Eval_Result &rhsEval = rhs->evaluate(eval_env, file_buffer);
    Data_Type dt = get_data_type();
    if (dt == int_data_type)
    {
        Eval_Result_Value_Int *num_eval = new Eval_Result_Value_Int();
        num_eval->set_value(lhsEval.get_int_value() / rhsEval.get_int_value());
        num_eval->set_result_enum(int_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
    else if (dt == double_data_type)
    {
        Eval_Result_Value_Double *num_eval = new Eval_Result_Value_Double();
        num_eval->set_value(lhsEval.get_double_value() / rhsEval.get_double_value());
        num_eval->set_result_enum(double_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
}
////////////////////////////////////////////////////////////
Eval_Result &UMinus_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsEval = lhs->evaluate(eval_env, file_buffer);
    Data_Type dt = get_data_type();
    if (dt == int_data_type)
    {
        Eval_Result_Value_Int *num_eval = new Eval_Result_Value_Int();
        num_eval->set_value(-lhsEval.get_int_value());
        num_eval->set_result_enum(int_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
    else if (dt == double_data_type)
    {
        Eval_Result_Value_Double *num_eval = new Eval_Result_Value_Double();
        num_eval->set_value(-lhsEval.get_double_value());
        num_eval->set_result_enum(double_result);
        num_eval->set_variable_status(true);
        return *num_eval;
    }
}
////////////////////////////////////////////////////////////
Eval_Result &Conditional_Expression_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &condEval = cond->evaluate(eval_env, file_buffer);
    int conditionNum = condEval.get_int_value();
    if (conditionNum == 1)
    {
        return lhs->evaluate(eval_env, file_buffer);
    }
    else
    {
        return rhs->evaluate(eval_env, file_buffer);
    }
}
////////////////////////////////////////////////////////////
Eval_Result &Return_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &eval_result = *static_cast<Eval_Result *>(NULL);
    return eval_result;
}
////////////////////////////////////////////////////////////
Eval_Result &Relational_Expr_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &lhsEval = lhs_condition->evaluate(eval_env, file_buffer);
    Eval_Result &rhsEval = rhs_condition->evaluate(eval_env, file_buffer);
    Data_Type dt = get_data_type();
    Eval_Result_Value_Int *bool_eval = new Eval_Result_Value_Int();
    int t;

    if (dt == int_data_type)
    {
        int lhsVal = lhsEval.get_int_value();
        int rhsVal = rhsEval.get_int_value();
        switch (rel_op)
        {
        case less_equalto:
            t = lhsVal <= rhsVal ? 1 : 0;
            break;
        case less_than:
            t = lhsVal < rhsVal ? 1 : 0;
            break;
        case greater_than:
            t = lhsVal > rhsVal ? 1 : 0;
            break;
        case greater_equalto:
            t = lhsVal >= rhsVal ? 1 : 0;
            break;
        case equalto:
            t = lhsVal == rhsVal ? 1 : 0;
            break;
        case not_equalto:
            t = lhsVal != rhsVal ? 1 : 0;
            break;
        }
    }
    else
    {
        double lhsVal = lhsEval.get_double_value();
        double rhsVal = rhsEval.get_double_value();
        switch (rel_op)
        {
        case less_equalto:
            t = lhsVal <= rhsVal ? 1 : 0;
            break;
        case less_than:
            t = lhsVal < rhsVal ? 1 : 0;
            break;
        case greater_than:
            t = lhsVal > rhsVal ? 1 : 0;
            break;
        case greater_equalto:
            t = lhsVal >= rhsVal ? 1 : 0;
            break;
        case equalto:
            t = lhsVal == rhsVal ? 1 : 0;
            break;
        case not_equalto:
            t = lhsVal != rhsVal ? 1 : 0;
            break;
        }
    }
    bool_eval->set_value(t);
    return *bool_eval;
}
////////////////////////////////////////////////////////////
Eval_Result &Logical_Expr_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{

    Data_Type dt = get_data_type();    
    int t=0;
    Eval_Result &lhsEval = lhs_op->evaluate(eval_env, file_buffer);
    int lhsVal = lhsEval.get_int_value();
    int rhsVal = 0;
    if (bool_op != _logical_not){
        Eval_Result &rhsEval = rhs_op->evaluate(eval_env, file_buffer);
        rhsVal = rhsEval.get_int_value();
    }    
    switch (bool_op)
    {
    case _logical_and:
        t = lhsVal && rhsVal ? 1 : 0;
        break;
    case _logical_or:
        t = lhsVal || rhsVal ? 1 : 0;
        break;
    case _logical_not:
        t = !lhsVal ? 1 : 0;
        break;
    }    
    Eval_Result_Value_Int *bool_eval = new Eval_Result_Value_Int();
    bool_eval->set_value(t);
    return *bool_eval;
}
////////////////////////////////////////////////////////////
Eval_Result &Selection_Statement_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    Eval_Result &condEval = cond->evaluate(eval_env, file_buffer);
    int conditionNum = condEval.get_int_value();
    if (conditionNum == 1)
    {
        return then_part->evaluate(eval_env, file_buffer);
    }
    else
    {
        if (else_part != NULL)
            return else_part->evaluate(eval_env, file_buffer);
        else
        {
            Eval_Result &rhsEval = *static_cast<Eval_Result *>(NULL);
            return rhsEval;
        }
    }
}
////////////////////////////////////////////////////////////
Eval_Result &Iteration_Statement_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    if (is_do_form)
    {
        body->evaluate(eval_env, file_buffer);
    }
    while (cond->evaluate(eval_env, file_buffer).get_int_value() == 1)
    {
        body->evaluate(eval_env, file_buffer);
    }
    Eval_Result &eval = *static_cast<Eval_Result *>(NULL);
    return eval;
}
////////////////////////////////////////////////////////////
Eval_Result &Sequence_Ast::evaluate(Local_Environment &eval_env, ostream &file_buffer)
{
    list<Ast *>::iterator it;
    for (it = statement_list.begin(); it != statement_list.end(); ++it)
    {
        (*it)->evaluate(eval_env, file_buffer);
    }
    Eval_Result &eval = *static_cast<Eval_Result *>(NULL);
    return eval;
}
