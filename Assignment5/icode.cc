

template class Const_Opd<double>;
template class Const_Opd<int>;

///////////////////////// Instruction Descriptor ///////////////////////////////////

Instruction_Descriptor::Instruction_Descriptor(){

}

Instruction_Descriptor::Instruction_Descriptor(Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	inst_op = op;
	mnemonic = mnn;
	ic_symbol = ics;       
	this->name = name;
	ic_format = icf;
	assem_format = af;	
}

Tgt_Op Instruction_Descriptor::get_op(){
	return inst_op;
}

string Instruction_Descriptor::get_name(){
	return name;
}

string Instruction_Descriptor::get_mnemonic(){
	return mnemonic;
}

string Instruction_Descriptor::get_ic_symbol(){
	return ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
	return ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){

}

///////////////////////////// Icode statement operand ///////////////////////////////////

Register_Descriptor * Ics_Opd::get_reg(){
	return NULL;
}

///////////////////////////////////////////////////////////////////

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){

}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){

}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs){
	// this->symbol_entry = rhs.symbol_entry;
	return *this; 
}

///////////////////////////////////////////////////////////////////

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	register_description = rd;
}

Register_Descriptor* Register_Addr_Opd::get_reg(){
	return register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){

}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){

}

Register_Addr_Opd & Register_Addr_Opd::operator= (const Register_Addr_Opd & rhs){
	// this->register_description = rhs.get_reg();
	return *this; 
}

///////////////////////////////////////////////////////////////////

template <class T>
Const_Opd<T>::Const_Opd (T num){
	this->num = num;
}

template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){

}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){

}

template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs){
	// this->num = rhs.num;
	return *this;
}

///////////////////////////////// Intermediate code statements //////////////////////////

Instruction_Descriptor & Icode_Stmt::get_op(){
	return op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){
	return NULL;
}
Ics_Opd * Icode_Stmt::get_opd2(){
	return NULL;
}
Ics_Opd * Icode_Stmt::get_result(){
	return NULL;
}

void Icode_Stmt::set_opd1(Ics_Opd * io){

}
void Icode_Stmt::set_opd2(Ics_Opd * io){

}
void Icode_Stmt::set_result(Ics_Opd * io){

}

///////////////////////////////////////////////////////////////////

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	this->opd1 = opd1;   
	this->result = result;
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}


Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	// opd1 = rhs.get_opd1();
	// result = rhs.get_result();
	// op_desc = machine_desc_object.spim_instruction_table[rhs.get_inst_op_of_ics().get_op()];
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}