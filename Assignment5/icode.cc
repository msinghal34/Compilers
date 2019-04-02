#include "icode.hh"
#include "reg-alloc.hh"
#include "ast.hh"
#include "symbol-table.hh"

template class Const_Opd<double>;
template class Const_Opd<int>;

///////////////////////// Instruction Descriptor ///////////////////////////////////

Instruction_Descriptor::Instruction_Descriptor()
{
}

Instruction_Descriptor::Instruction_Descriptor(Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af)
{
	inst_op = op;
	mnemonic = mnn;
	ic_symbol = ics;
	this->name = name;
	ic_format = icf;
	assem_format = af;
}

Tgt_Op Instruction_Descriptor::get_op()
{
	return inst_op;
}

string Instruction_Descriptor::get_name()
{
	return name;
}

string Instruction_Descriptor::get_mnemonic()
{
	return mnemonic;
}

string Instruction_Descriptor::get_ic_symbol()
{
	return ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format()
{
	return ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format()
{
	return assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream &file_buffer)
{
}

///////////////////////////// Icode statement operand ///////////////////////////////////

Register_Descriptor *Ics_Opd::get_reg()
{
	return NULL;
}

///////////////////////////////////////////////////////////////////

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry &se)
{
	symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream &file_buffer)
{
	file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream &file_buffer)
{
	file_buffer << symbol_entry->get_start_offset() << "($" << (machine_desc_object.spim_register_table[fp])->get_name() << ")";
}

Mem_Addr_Opd &Mem_Addr_Opd::operator=(const Mem_Addr_Opd &rhs)
{
	symbol_entry = rhs.symbol_entry;
	return *this;
}

///////////////////////////////////////////////////////////////////

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor *rd)
{
	register_description = rd;
}

Register_Descriptor *Register_Addr_Opd::get_reg()
{
	return register_description;
}

void Register_Addr_Opd::print_ics_opd(ostream &file_buffer)
{
	file_buffer << register_description->get_name();
}

void Register_Addr_Opd::print_asm_opd(ostream &file_buffer)
{
	file_buffer << "$" << register_description->get_name();
}

Register_Addr_Opd &Register_Addr_Opd::operator=(const Register_Addr_Opd &rhs)
{
	register_description == rhs.register_description;
	return *this;
}

///////////////////////////////////////////////////////////////////

template <class T>
Const_Opd<T>::Const_Opd(T num)
{
	this->num = num;
}

template <class T>
void Const_Opd<T>::print_ics_opd(ostream &file_buffer)
{
	file_buffer << this->num;
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream &file_buffer)
{
	file_buffer << this->num;
}

template <class T>
Const_Opd<T> &Const_Opd<T>::operator=(const Const_Opd &rhs)
{
	this->num = rhs.num;
	return *this;
}

///////////////////////////////// Intermediate code statements //////////////////////////

Instruction_Descriptor &Icode_Stmt::get_op()
{
	return op_desc;
}

Ics_Opd *Icode_Stmt::get_opd1()
{
	return NULL;
}
Ics_Opd *Icode_Stmt::get_opd2()
{
	return NULL;
}
Ics_Opd *Icode_Stmt::get_result()
{
	return NULL;
}

void Icode_Stmt::set_opd1(Ics_Opd *io)
{
}
void Icode_Stmt::set_opd2(Ics_Opd *io)
{
}
void Icode_Stmt::set_result(Ics_Opd *io)
{
}

///////////////////////////////////////////////////////////////////

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd *opd1, Ics_Opd *result)
{
	this->opd1 = opd1;
	this->result = result;
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Move_IC_Stmt &Move_IC_Stmt::operator=(const Move_IC_Stmt &rhs)
{
	opd1 = rhs.opd1;
	op_desc = rhs.op_desc;
	result = rhs.result;
	return *this;
}

Instruction_Descriptor &Move_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd *Move_IC_Stmt::get_opd1()
{
	return opd1;
}
void Move_IC_Stmt::set_opd1(Ics_Opd *io)
{
	opd1 = io;
}

Ics_Opd *Move_IC_Stmt::get_result()
{
	return result;
}
void Move_IC_Stmt::set_result(Ics_Opd *io)
{
	result = io;
}

void Move_IC_Stmt::print_icode(ostream &file_buffer)
{

	file_buffer << AST_SPACE << op_desc.get_name() << ":    \t";
	result->print_ics_opd(file_buffer);
	file_buffer << " <- ";
	opd1->print_ics_opd(file_buffer);
	file_buffer << endl;
}

void Move_IC_Stmt::print_assembly(ostream &file_buffer)
{
	file_buffer << AST_SPACE << op_desc.get_mnemonic() << " ";
	Tgt_Op op = op_desc.get_op();
	if (op == store || op == store_d)
	{
		opd1->print_asm_opd(file_buffer);
		file_buffer << ", ";
		result->print_asm_opd(file_buffer);
		file_buffer << endl;
	}
	else // All load
	{
		result->print_asm_opd(file_buffer);
		file_buffer << ", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer << endl;
	}
}
///////////////////////////////////////////////////////////////////

Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd *opd1, Ics_Opd *opd2, Ics_Opd *result)
{
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Instruction_Descriptor &Compute_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Compute_IC_Stmt &Compute_IC_Stmt::operator=(const Compute_IC_Stmt &rhs)
{
	opd1 = rhs.opd1;
	opd2 = rhs.opd2;
	result = rhs.result;
	op_desc = rhs.op_desc;
	return *this;
}

Ics_Opd *Compute_IC_Stmt::get_opd1()
{
	return opd1;
}
void Compute_IC_Stmt::set_opd1(Ics_Opd *io)
{
	opd1 = io;
}

Ics_Opd *Compute_IC_Stmt::get_opd2()
{
	return opd2;
}
void Compute_IC_Stmt::set_opd2(Ics_Opd *io)
{
	opd2 = io;
}

Ics_Opd *Compute_IC_Stmt::get_result()
{
	return result;
}
void Compute_IC_Stmt::set_result(Ics_Opd *io)
{
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream &file_buffer)
{
	file_buffer << AST_SPACE << op_desc.get_name() << ":    \t";
	result->print_ics_opd(file_buffer);
	file_buffer << " <- ";
	opd1->print_ics_opd(file_buffer);
	if (opd2 != NULL)
	{
		file_buffer << " , ";
		opd2->print_ics_opd(file_buffer);
	}
	file_buffer << endl;
}

void Compute_IC_Stmt::print_assembly(ostream &file_buffer)
{
	file_buffer << AST_SPACE << op_desc.get_mnemonic() << " ";
	result->print_asm_opd(file_buffer);
	file_buffer << ", ";
	opd1->print_asm_opd(file_buffer);
	if (opd2 != NULL)
	{
		file_buffer << ", ";
		opd2->print_asm_opd(file_buffer);
	}
	file_buffer << endl;
}
///////////////////////////////////////////////////////////////////

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd *opd1, string label)
{
	this->opd1 = opd1;
	this->label = label;
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Control_Flow_IC_Stmt &Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt &rhs)
{
	opd1 = rhs.opd1;
	label = rhs.label;
	op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor &Control_Flow_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

Ics_Opd *Control_Flow_IC_Stmt::get_opd1()
{
	return opd1;
}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd *io)
{
	opd1 = io;
}

string Control_Flow_IC_Stmt::get_label()
{
	return label;
}
void Control_Flow_IC_Stmt::set_label(string label)
{
	this->label = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream &file_buffer)
{
	switch (op_desc.get_ic_format())
	{
		case i_op_o1_o2_st:
			file_buffer << AST_SPACE << op_desc.get_name() << ":    \t";
			opd1->print_ics_opd(file_buffer);
			file_buffer<<" , zero : goto ";
			file_buffer<<label<<endl;
			break;
		case i_op_st:
			file_buffer << "goto " << label << endl;
			break;

		default:
			file_buffer << AST_SPACE << op_desc.get_name() << "\n";
			break;
	}	
}
void Control_Flow_IC_Stmt::print_assembly(ostream &file_buffer)
{
	file_buffer << AST_SPACE << op_desc.get_mnemonic() << " ";
	switch (op_desc.get_assembly_format())
	{
		case a_op_o1_o2_st:
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			file_buffer<<"$"<<machine_desc_object.spim_register_table[zero]->get_name();
			file_buffer << ", "<<label<<endl;
			break;
		case i_op_st:
			file_buffer << label << endl;
			break;

		default:
			break;
	}
}
///////////////////////////////////////////////////////////////////

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label)
{
	this->label = label;
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];
}

Label_IC_Stmt &Label_IC_Stmt::operator=(const Label_IC_Stmt &rhs)
{
	label = rhs.label;
	op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor &Label_IC_Stmt::get_inst_op_of_ics()
{
	return op_desc;
}

string Label_IC_Stmt::get_label()
{
	return label;
}

void Label_IC_Stmt::set_label(string label)
{
	this->label = label;
}

void Label_IC_Stmt::print_icode(ostream &file_buffer)
{
	file_buffer << label << ":\n";
}

void Label_IC_Stmt::print_assembly(ostream &file_buffer)
{
	file_buffer << label << ":\n";
}

//////////////////////// Intermediate code for Ast statements ////////////////////////

Code_For_Ast::Code_For_Ast()
{
}
Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> &ic_l, Register_Descriptor *reg)
{
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt &ics)
{
	ics_list.push_back(&ics);
} 
list<Icode_Stmt *> &Code_For_Ast::get_icode_list()
{
	return ics_list;
}

Register_Descriptor *Code_For_Ast::get_reg()
{
	return result_register;
}
void Code_For_Ast::set_reg(Register_Descriptor *reg)
{
	result_register = reg;
}

Code_For_Ast &Code_For_Ast::operator=(const Code_For_Ast &rhs)
{
	result_register = rhs.result_register;
	ics_list = rhs.ics_list;
	return *this;
}