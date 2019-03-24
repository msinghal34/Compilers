#include "icode.hh"
#include "reg-alloc.hh"
#include "ast.hh"
#include "symbol-table.hh"

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){
    return *(new Code_For_Ast());
}

Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast *rhs_code = &(rhs->compile());
	Register_Descriptor *reg = rhs_code->get_reg();
	Code_For_Ast *lhs_code = &(lhs->create_store_stmt(rhs_code->get_reg()));
	reg->reset_use_for_expr_result();
	list<Icode_Stmt *> *rhs_icode_list = &rhs_code->get_icode_list();
	list<Icode_Stmt *> *lhs_icode_list = &lhs_code->get_icode_list();
	rhs_icode_list->merge(*lhs_icode_list);

	Code_For_Ast *icode = new Code_For_Ast(*rhs_icode_list,reg);
	return *icode;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	return *(new Code_For_Ast());
}


//  Move IC(instr,opd,result) Convention we are following
Code_For_Ast & Name_Ast::compile(){
	if(node_data_type == int_data_type){
		Register_Descriptor * reg = machine_desc_object.get_new_register<gp_data>();
		reg->set_register_occupied()
		Mem_Addr_Opd *m_opd = new Mem_Addr_Opd(get_symbol_entry());
		Register_Addr_Opd *regopd = new Register_Addr_Opd(reg);
		Move_IC_Stmt *move = new Move_IC_Stmt(load,m_opd,regopd);
		list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
		Code_For_Ast *icode = new Code_For_Ast(*ics_list,reg);
		icode->append_ics(*move);
		return *icode;
	}
	else{
		Register_Descriptor * reg = machine_desc_object.get_new_register<float_reg>();
		reg->set_register_occupied()
		Mem_Addr_Opd *m_opd = new Mem_Addr_Opd(get_symbol_entry());
		Register_Addr_Opd *regopd = new Register_Addr_Opd(reg);
		Move_IC_Stmt *move = new Move_IC_Stmt(load_d,m_opd,regopd);
		list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
		Code_For_Ast *icode = new Code_For_Ast(*ics_list,reg);
		icode->append_ics(*move);
		return *icode;
	}
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	return *(new Code_For_Ast());
}

Code_For_Ast & create_store_stmt(Register_Descriptor * store_register){
	Mem_Addr_Opd *m_opd = new Mem_Addr_Opd(get_symbol_entry());
	Register_Addr_Opd *reg_opd = new Register_Addr_Opd(store_register);
	if(node_data_type == int_data_type){
		Move_IC_Stmt *move = new Move_IC_Stmt(store,reg_opd,m_opd);
		list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
		Code_For_Ast *icode = new Code_For_Ast(*ics_list,reg);
		icode->append_ics(*move);
		return *icode;
	}
	else{
		Move_IC_Stmt *move = new Move_IC_Stmt(store_d,reg_opd,m_opd);
		list<Icode_Stmt *> *ics_list = new list<Icode_Stmt *>();
		Code_For_Ast *icode = new Code_For_Ast(*ics_list,reg);
		icode->append_ics(*move);
		return *icode;
	}
	
}