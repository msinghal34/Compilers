#include "icode.hh"
#include "reg-alloc.hh"
#include "ast.hh"
#include "symbol-table.hh"

#include<algorithm>
#include<iterator>
#include<list>

template class Number_Ast<double>;
template class Number_Ast<int>;
///////////////////////// Ast ////////////////////////
Code_For_Ast &Ast::create_store_stmt(Register_Descriptor *store_register)
{
	return *(new Code_For_Ast());
}

///////////////////////// Assignment_Ast ////////////////////////
Code_For_Ast &Assignment_Ast::compile()
{
	Code_For_Ast *rhs_code = &(rhs->compile());
	Register_Descriptor *reg = rhs_code->get_reg();
	Code_For_Ast *lhs_code = &(lhs->create_store_stmt(rhs_code->get_reg()));
	reg->reset_use_for_expr_result();
	list<Icode_Stmt *> *rhs_icode_list = &rhs_code->get_icode_list();
	list<Icode_Stmt *> *lhs_icode_list = &lhs_code->get_icode_list();
	rhs_icode_list->merge(*lhs_icode_list);
	Code_For_Ast *icode = new Code_For_Ast(*rhs_icode_list, reg);
	return *icode;
}

Code_For_Ast &Assignment_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
}

///////////////////////// Name_Ast ////////////////////////
Code_For_Ast &Name_Ast::compile()
{
	Register_Descriptor *reg;
	Tgt_Op ltype;
	if (node_data_type == int_data_type)
	{
		reg = machine_desc_object.get_new_register<int_reg>();
		ltype = load;
	}
	else if (node_data_type == double_data_type)
	{
		reg = machine_desc_object.get_new_register<float_reg>();
		ltype = load_d;
	}
	else
	{
		cout << "Internal error! Void data type" << endl;
		exit(0);
	}

	reg->set_use_for_expr_result();
	Mem_Addr_Opd *m_opd = new Mem_Addr_Opd(get_symbol_entry());
	Register_Addr_Opd *regopd = new Register_Addr_Opd(reg);
	Move_IC_Stmt *move = new Move_IC_Stmt(ltype, m_opd, regopd);
	list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
	Code_For_Ast *icode = new Code_For_Ast(*ics_list, reg);
	icode->append_ics(*move);
	return *icode;
}

Code_For_Ast &Name_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}

Code_For_Ast &Name_Ast::create_store_stmt(Register_Descriptor *store_register)
{
	Mem_Addr_Opd *m_opd = new Mem_Addr_Opd(get_symbol_entry());
	Register_Addr_Opd *reg_opd = new Register_Addr_Opd(store_register);
	list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
	Code_For_Ast *icode = new Code_For_Ast(*ics_list, store_register);
	Move_IC_Stmt *move = new Move_IC_Stmt(node_data_type == int_data_type ? store : store_d, reg_opd, m_opd);
	icode->append_ics(*move);
	return *icode;
}

///////////////////////// Number_Ast ////////////////////////

template <class T>
Code_For_Ast &Number_Ast<T>::compile()
{
	Register_Descriptor *store_register;
	Move_IC_Stmt *move;
	if (node_data_type == int_data_type)
	{
		store_register = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd *reg_opd = new Register_Addr_Opd(store_register);
		Const_Opd<int> *const_opd = new Const_Opd<int>(constant);
		move = new Move_IC_Stmt(imm_load, const_opd, reg_opd);
	}
	else if (node_data_type == double_data_type)
	{
		store_register = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd *reg_opd = new Register_Addr_Opd(store_register);
		Const_Opd<double> *const_opd = new Const_Opd<double>(constant);
		move = new Move_IC_Stmt(imm_load_d, const_opd, reg_opd);
	}
	else
	{
		cout << "Internal error! Void data type" << endl;
		exit(0);
	}
	store_register->set_use_for_expr_result();
	list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
	Code_For_Ast *icode = new Code_For_Ast(*ics_list, store_register);
	icode->append_ics(*move);
	return *icode;
}

template <class T>
Code_For_Ast &Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}

///////////////////////// Plus_Ast ////////////////////////
Code_For_Ast &Plus_Ast::compile()
{
	Code_For_Ast &lcode = lhs->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Code_For_Ast &rcode = rhs->compile();
	Register_Descriptor *rreg = rcode.get_reg();
	Register_Addr_Opd *rropd = new Register_Addr_Opd(rreg);

	Register_Descriptor *result_reg;
	Tgt_Op op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		op = add;
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		op = add_d;
	}
	lreg->reset_use_for_expr_result();
	rreg->reset_use_for_expr_result();

	result_reg->set_use_for_expr_result();
	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	Compute_IC_Stmt *ic_stmt = new Compute_IC_Stmt(op, lropd, rropd, result_opd);

	list<Icode_Stmt *> llist = lcode.get_icode_list();
	list<Icode_Stmt *> rlist = rcode.get_icode_list();
	llist.merge(rlist);
	Code_For_Ast *result_code = new Code_For_Ast(llist, result_reg);
	result_code->append_ics(*ic_stmt);
	return *result_code;
}
Code_For_Ast &Plus_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}
///////////////////////// Minus_Ast ////////////////////////
Code_For_Ast &Minus_Ast::compile()
{
	Code_For_Ast &lcode = lhs->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Code_For_Ast &rcode = rhs->compile();
	Register_Descriptor *rreg = rcode.get_reg();
	Register_Addr_Opd *rropd = new Register_Addr_Opd(rreg);
	Register_Descriptor *result_reg;
	Tgt_Op op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		op = sub;
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		op = sub_d;
	}
	lreg->reset_use_for_expr_result();
	rreg->reset_use_for_expr_result();

	result_reg->set_use_for_expr_result();
	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	Compute_IC_Stmt *ic_stmt = new Compute_IC_Stmt(op, lropd, rropd, result_opd);

	list<Icode_Stmt *> llist = lcode.get_icode_list();
	list<Icode_Stmt *> rlist = rcode.get_icode_list();
	llist.merge(rlist);
	Code_For_Ast *result_code = new Code_For_Ast(llist, result_reg);
	result_code->append_ics(*ic_stmt);
	return *result_code;
}
Code_For_Ast &Minus_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}
///////////////////////// Mult_Ast ////////////////////////
Code_For_Ast &Mult_Ast::compile()
{
	Code_For_Ast &lcode = lhs->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Code_For_Ast &rcode = rhs->compile();
	Register_Descriptor *rreg = rcode.get_reg();
	Register_Addr_Opd *rropd = new Register_Addr_Opd(rreg);

	Register_Descriptor *result_reg;
	Tgt_Op op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		op = mult;
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		op = mult_d;
	}

	lreg->reset_use_for_expr_result();
	rreg->reset_use_for_expr_result();

	result_reg->set_use_for_expr_result();
	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	Compute_IC_Stmt *ic_stmt = new Compute_IC_Stmt(op, lropd, rropd, result_opd);

	list<Icode_Stmt *> llist = lcode.get_icode_list();
	list<Icode_Stmt *> rlist = rcode.get_icode_list();
	llist.merge(rlist);
	Code_For_Ast *result_code = new Code_For_Ast(llist, result_reg);
	result_code->append_ics(*ic_stmt);
	return *result_code;
}
Code_For_Ast &Mult_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}
///////////////////////// Divide_Ast ////////////////////////
Code_For_Ast &Divide_Ast::compile()
{
	Code_For_Ast &lcode = lhs->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Code_For_Ast &rcode = rhs->compile();
	Register_Descriptor *rreg = rcode.get_reg();
	Register_Addr_Opd *rropd = new Register_Addr_Opd(rreg);

	Register_Descriptor *result_reg;
	Tgt_Op op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		op = divd;
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		op = div_d;
	}

	lreg->reset_use_for_expr_result();
	rreg->reset_use_for_expr_result();

	result_reg->set_use_for_expr_result();
	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	Compute_IC_Stmt *ic_stmt = new Compute_IC_Stmt(op, lropd, rropd, result_opd);

	list<Icode_Stmt *> llist = lcode.get_icode_list();
	list<Icode_Stmt *> rlist = rcode.get_icode_list();
	llist.merge(rlist);
	Code_For_Ast *result_code = new Code_For_Ast(llist, result_reg);
	result_code->append_ics(*ic_stmt);
	return *result_code;
}
Code_For_Ast &Divide_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}
///////////////////////// UMinus_Ast ////////////////////////
Code_For_Ast &UMinus_Ast::compile()
{
	Code_For_Ast &lcode = lhs->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Register_Descriptor *result_reg;
	Tgt_Op op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		op = uminus;
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		op = uminus_d;
	}

	lreg->reset_use_for_expr_result();

	result_reg->set_use_for_expr_result();
	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	Compute_IC_Stmt *ic_stmt = new Compute_IC_Stmt(op, lropd, NULL, result_opd);

	list<Icode_Stmt *> llist = lcode.get_icode_list();
	Code_For_Ast *result_code = new Code_For_Ast(llist, result_reg);
	result_code->append_ics(*ic_stmt);
	return *result_code;
}
Code_For_Ast &UMinus_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}

///////////////////////// Conditional_Expression_Ast ////////////////////////

void merge_list(list<Icode_Stmt *> &l1, const list<Icode_Stmt *> &l2)
{
	l1.insert(l1.end(), l2.begin(), l2.end());
}


Code_For_Ast &Conditional_Expression_Ast::compile()
{
	Code_For_Ast &cond_code = cond->compile();
	Register_Descriptor *cond_reg = cond_code.get_reg();
	Register_Addr_Opd *cond_opd = new Register_Addr_Opd(cond_reg);

	Code_For_Ast &lcode = lhs->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Code_For_Ast &rcode = rhs->compile();
	Register_Descriptor *rreg = rcode.get_reg();
	Register_Addr_Opd *rropd = new Register_Addr_Opd(rreg);

	Register_Descriptor *result_reg;
	Tgt_Op move_op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		move_op = mov;
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		move_op = move_d;
	}

	cond_reg->reset_use_for_expr_result();
	lreg->reset_use_for_expr_result();
	rreg->reset_use_for_expr_result();
	result_reg->set_use_for_expr_result();

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	string else_start = get_new_label();
	string else_end = get_new_label();

	Control_Flow_IC_Stmt *beq_stmt = new Control_Flow_IC_Stmt(beq, cond_opd, else_start);
	Control_Flow_IC_Stmt *goto_else_end = new Control_Flow_IC_Stmt(j, NULL, else_end);
	Label_IC_Stmt *label_else_start = new Label_IC_Stmt(label, else_start);
	Label_IC_Stmt *label_else_end = new Label_IC_Stmt(label, else_end);
	Move_IC_Stmt *if_part = new Move_IC_Stmt(move_op, lropd, result_opd);
	Move_IC_Stmt *else_part = new Move_IC_Stmt(move_op, rropd, result_opd);

	list<Icode_Stmt *> ics_list = cond_code.get_icode_list();
	ics_list.insert(ics_list.end(), lcode.get_icode_list().begin(), lcode.get_icode_list().end());
	ics_list.push_back(beq_stmt);
	ics_list.push_back(if_part);
	ics_list.push_back(goto_else_end);
	ics_list.push_back(label_else_start);
	ics_list.insert(ics_list.end(), rcode.get_icode_list().begin(), rcode.get_icode_list().end()); 
	ics_list.push_back(else_part);
	ics_list.push_back(label_else_end);
	return *(new Code_For_Ast(ics_list, result_reg));
}

///////////////////////// Return_Ast ////////////////////////

Code_For_Ast &Return_Ast::compile()
{
	return *(new Code_For_Ast());
}

Code_For_Ast &Return_Ast::compile_and_optimize_ast(Lra_Outcome &lra)
{
	return *(new Code_For_Ast());
}

///////////////////////// Relational_Expr_Ast ////////////////////////

Code_For_Ast &Relational_Expr_Ast::compile()
{
	Code_For_Ast &lcode = lhs_condition->compile();
	Register_Descriptor *lreg = lcode.get_reg();
	Register_Addr_Opd *lropd = new Register_Addr_Opd(lreg);

	Code_For_Ast &rcode = rhs_condition->compile();
	Register_Descriptor *rreg = rcode.get_reg();
	Register_Addr_Opd *rropd = new Register_Addr_Opd(rreg);

	Tgt_Op Relop_array[] = {sle, slt, sgt, sge, seq, sne};
	Tgt_Op Relop_array_d[] = {sle_d, slt_d, sgt_d, sge_d, seq_d, sne_d};

	Register_Descriptor *result_reg;
	Tgt_Op op;
	if (node_data_type == int_data_type)
	{
		result_reg = machine_desc_object.get_new_register<int_reg>();
		op = Relop_array[rel_op];
	}
	else
	{
		result_reg = machine_desc_object.get_new_register<float_reg>();
		op = Relop_array_d[rel_op];
	}

	lreg->reset_use_for_expr_result();
	rreg->reset_use_for_expr_result();
	result_reg->set_use_for_expr_result();

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(result_reg);

	Compute_IC_Stmt *ic_stmt = new Compute_IC_Stmt(op, lropd, rropd, result_opd);

	list<Icode_Stmt *> llist = lcode.get_icode_list();
	list<Icode_Stmt *> rlist = rcode.get_icode_list();
	llist.merge(rlist);
	Code_For_Ast *result_code = new Code_For_Ast(llist, result_reg);
	result_code->append_ics(*ic_stmt);
	return *result_code;
}

///////////////////////// Logical_Expr_Ast ////////////////////////

Code_For_Ast &Logical_Expr_Ast::compile()
{
	return *(new Code_For_Ast());
}
///////////////////////// Selection_Statement_Ast ////////////////////////

Code_For_Ast &Selection_Statement_Ast::compile()
{
	return *(new Code_For_Ast());
}

///////////////////////// Iteration_Statement_Ast ////////////////////////

Code_For_Ast &Iteration_Statement_Ast::compile()
{
	return *(new Code_For_Ast());
}

///////////////////////// Sequence_Ast ////////////////////////

Code_For_Ast &Sequence_Ast::compile()
{
	return *(new Code_For_Ast());
}
///////////////////////// Print_Ast ////////////////////////

// Code_For_Ast & Print_Ast::compile()
// {
// 	return *(new Code_For_Ast());
// }
