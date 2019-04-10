
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "local-environment.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

Machine_Description machine_desc_object;

//////////////////////////// Register Descriptor ///////////////////////////////

Register_Descriptor::Register_Descriptor(Spim_Register reg, string s, Register_Val_Type vt, Register_Use_Category uc)
{
	reg_id = reg;
	reg_name = s;
	value_type = vt;
	reg_use = uc; 
	used_for_expr_result = false;
	reg_occupied = false;
	used_for_fn_return = false;
}

Register_Use_Category Register_Descriptor::get_use_category()	{ return reg_use; }
Spim_Register Register_Descriptor::get_register()		{ return reg_id; }
string Register_Descriptor::get_name()				{ return reg_name; }
bool Register_Descriptor::is_symbol_list_empty()		{ return lra_symbol_list.empty(); }

bool Register_Descriptor::is_register_occupied()		{ return reg_occupied; }
void Register_Descriptor::set_register_occupied()		{ reg_occupied = true; }
void Register_Descriptor::reset_register_occupied()		{ reg_occupied = false; }

bool Register_Descriptor::is_used_for_expr_result()		{ return used_for_expr_result; }
void Register_Descriptor::reset_use_for_expr_result()		{ used_for_expr_result = false; 
								  reg_occupied = false; 
								  used_for_fn_return = false; }
void Register_Descriptor::set_use_for_expr_result()		{ used_for_expr_result = true; }

void Register_Descriptor::set_used_for_fn_return()		{ used_for_fn_return = true; }
void Register_Descriptor::reset_used_for_fn_return()		{ used_for_fn_return = false; }
bool Register_Descriptor::is_used_for_fn_return()		{ return used_for_fn_return; }

int Register_Descriptor::count_symbol_entry_in_list()
{
	int count = 0;

	list<Symbol_Table_Entry *>::iterator i;
	for(i = lra_symbol_list.begin(); i != lra_symbol_list.end(); i++)
		count++;

	return count;
}

template <Register_Use_Category dt>
bool Register_Descriptor::is_free()     
{
	if ((reg_use == dt) && 
	(lra_symbol_list.empty()) && 
	(is_used_for_expr_result() == false) && 
	(is_register_occupied() == false)) 
		return true;
	else 
		return false;
}

void Register_Descriptor::remove_symbol_entry_from_list(Symbol_Table_Entry & sym_entry)
{
	lra_symbol_list.remove(&sym_entry);
}

bool Register_Descriptor::find_symbol_entry_in_list(Symbol_Table_Entry & sym_entry)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = lra_symbol_list.begin(); i != lra_symbol_list.end(); i++)
		if (**i == sym_entry)
			return true;

	return false;
}

void Register_Descriptor::clear_lra_symbol_list()
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = lra_symbol_list.begin(); i != lra_symbol_list.end(); i++)
	{
		Symbol_Table_Entry & sym_entry = **i;
		sym_entry.set_register(NULL);
	}

	lra_symbol_list.clear();
}

void Register_Descriptor::update_symbol_information(Symbol_Table_Entry & sym_entry)
{
	if (find_symbol_entry_in_list(sym_entry) == false)
		lra_symbol_list.push_back(&sym_entry);
}

//////////////////////////////// Lra_Outcome //////////////////////////////////////////

Lra_Outcome::Lra_Outcome(Register_Descriptor * rdp, bool nr, bool sr, bool dr, bool mv, bool ld)
{
	register_description = rdp;
	is_a_new_register = nr;
	is_same_as_source = sr;
	is_same_as_destination = dr;
	register_move_needed = mv; 
	load_needed = ld;
}

Lra_Outcome::Lra_Outcome()
{
	register_description = NULL;
	is_a_new_register = false;
	is_same_as_source = false;
	is_same_as_destination = false;
	register_move_needed = false;
	load_needed = false;
}


Register_Descriptor * Lra_Outcome::get_register() 	{ return register_description; }
bool Lra_Outcome::is_new_register()      		{ return is_a_new_register; }
bool Lra_Outcome::is_source_register()   		{ return is_same_as_source; }
bool Lra_Outcome::is_destination_register()    		{ return is_same_as_destination; }
bool Lra_Outcome::is_move_needed()      		{ return register_move_needed; }
bool Lra_Outcome::is_load_needed()	     		{ return load_needed; }

template <Register_Use_Category dt>
void Lra_Outcome::optimize_lra(Lra_Scenario lcase, Ast * destination_memory, Ast * source_memory)
{
	// Register allocation is done only when the source is in either memory or is a constant

	Register_Descriptor * destination_register, * source_register, * result_register;
	Symbol_Table_Entry * source_symbol_entry, * destination_symbol_entry;

	destination_register = NULL;
	source_register = NULL;
	result_register = NULL;

	is_a_new_register = false;
	is_same_as_source = false;
	is_same_as_destination = false;
	register_move_needed = false;
	load_needed = false;

	switch (lcase)
	{
	case mc_2m:
		if (destination_memory != NULL)
		{
			CHECK_INVARIANT((typeid(*destination_memory) != typeid(Number_Ast<int>) || (typeid(*destination_memory) != typeid(Number_Ast<double>))),
				"Destination memory cannot be a constant");

			destination_symbol_entry = &(destination_memory->get_symbol_entry());
			destination_register = destination_symbol_entry->get_register(); 
		}
		else
			destination_register = NULL;

		CHECK_INVARIANT(source_memory, 
			"Sourse ast pointer cannot be NULL for m2m scenario in lra");
		if (typeid(*source_memory) == typeid(Number_Ast<int>) || (typeid(*source_memory) == typeid(Number_Ast<double>)))
			source_register = NULL;
		else
		{
			source_symbol_entry = &(source_memory->get_symbol_entry());
			source_register = source_symbol_entry->get_register(); 
		}

		if (source_register != NULL)
		{
			result_register = source_register;
			is_same_as_source = true;
			load_needed = false;
		}
		else if (destination_register != NULL)
		{
			int count_var = destination_register->count_symbol_entry_in_list();
			if (count_var > 1)
			{
				result_register = machine_desc_object.get_new_register<dt>();
				is_a_new_register = true;
			}

			else
			{
				result_register = destination_register;
				is_same_as_destination = true;
			}

			load_needed = true;
		}
		else 
		{
			result_register = machine_desc_object.get_new_register<dt>();
			is_a_new_register = true;
			load_needed = true;
		}

		break;

	case mc_2r:
		CHECK_INVARIANT(source_memory, "Sourse ast pointer cannot be NULL for m2r scenario in lra");

 		if (typeid(*source_memory) == typeid(Number_Ast<int>) || (typeid(*source_memory) == typeid(Number_Ast<double>)))
			source_register = NULL;
		else
		{
			source_symbol_entry = &(source_memory->get_symbol_entry());
			source_register = source_symbol_entry->get_register(); 
		}

		if (source_register != NULL)
		{
			result_register = source_register;
			is_same_as_source = true;
			load_needed = false;
		}
		else 
		{
			result_register = machine_desc_object.get_new_register<dt>();
			is_a_new_register = true;
			load_needed = true;
		}

		break;

	case r2r:
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"r2r scenario does not call for local register allocation");

		break;	

	case r2m:
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"r2m scenario does not call for local register allocation. Register allocation only allowed in mc_2m or mc_2r. So register allocation is not required in r2m");

		break;

	default:
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
				"Illegal local register allocation scenario");

		break;
	}

	CHECK_INVARIANT ((result_register != NULL), "Inconsistent information in lra");
	register_description = result_register;

	// Assign register to source memory
	if ((source_register) && 
	(typeid(*source_memory) != typeid(Number_Ast<int>) && 
	(typeid(*source_memory) != typeid(Number_Ast<double>))) && 
	(source_memory != NULL))
	{
		source_symbol_entry->free_register(source_register);
	}

	if ((source_memory != NULL) && 
	(typeid(*source_memory) != typeid(Number_Ast<int>) &&
	(typeid(*source_memory) != typeid(Number_Ast<double>))))
	{
		source_symbol_entry->update_register(result_register);
	}

	if ((destination_register) && (destination_memory != NULL))
		destination_symbol_entry->free_register(destination_register); 

	if ((result_register != NULL) && (destination_memory != NULL))
		destination_symbol_entry->update_register(result_register);
}

/******************************* Machine Description *****************************************/

void Machine_Description::initialize_register_table()
{
	spim_register_table[zero] = new Register_Descriptor(zero, "zero", int_num, fixed_reg);
	spim_register_table[v0] = new Register_Descriptor(v0, "v0", int_num, int_reg);
	spim_register_table[v1] = new Register_Descriptor(v1, "v1", int_num, fn_result);
	spim_register_table[a0] = new Register_Descriptor(a0, "a0", int_num, argument);
	spim_register_table[a1] = new Register_Descriptor(a1, "a1", int_num, argument);
	spim_register_table[a2] = new Register_Descriptor(a2, "a2", int_num, argument);
	spim_register_table[a3] = new Register_Descriptor(a3, "a3", int_num, argument);
	spim_register_table[t0] = new Register_Descriptor(t0, "t0", int_num, int_reg);
	spim_register_table[t1] = new Register_Descriptor(t1, "t1", int_num, int_reg);
	spim_register_table[t2] = new Register_Descriptor(t2, "t2", int_num, int_reg);
	spim_register_table[t3] = new Register_Descriptor(t3, "t3", int_num, int_reg);
	spim_register_table[t4] = new Register_Descriptor(t4, "t4", int_num, int_reg);
	spim_register_table[t5] = new Register_Descriptor(t5, "t5", int_num, int_reg);
	spim_register_table[t6] = new Register_Descriptor(t6, "t6", int_num, int_reg);
	spim_register_table[t7] = new Register_Descriptor(t7, "t7", int_num, int_reg);
	spim_register_table[t8] = new Register_Descriptor(t8, "t8", int_num, int_reg);
	spim_register_table[t9] = new Register_Descriptor(t9, "t9", int_num, int_reg);
	spim_register_table[s0] = new Register_Descriptor(s0, "s0", int_num, int_reg);
	spim_register_table[s1] = new Register_Descriptor(s1, "s1", int_num, int_reg);
	spim_register_table[s2] = new Register_Descriptor(s2, "s2", int_num, int_reg);
	spim_register_table[s3] = new Register_Descriptor(s3, "s3", int_num, int_reg);
	spim_register_table[s4] = new Register_Descriptor(s4, "s4", int_num, int_reg);
	spim_register_table[s5] = new Register_Descriptor(s5, "s5", int_num, int_reg);
	spim_register_table[s6] = new Register_Descriptor(s6, "s6", int_num, int_reg);
	spim_register_table[s7] = new Register_Descriptor(s7, "s7", int_num, int_reg);
	spim_register_table[f0] = new Register_Descriptor(f0, "f0", float_num, fn_result);
	spim_register_table[f2] = new Register_Descriptor(f2, "f2", float_num, float_reg);
	spim_register_table[f4] = new Register_Descriptor(f4, "f4", float_num, float_reg);
	spim_register_table[f6] = new Register_Descriptor(f6, "f6", float_num, float_reg);
	spim_register_table[f8] = new Register_Descriptor(f8, "f8", float_num, float_reg);
	spim_register_table[f10] = new Register_Descriptor(f10, "f10", float_num, float_reg);
	spim_register_table[f12] = new Register_Descriptor(f12, "f12", float_num, float_reg);
	spim_register_table[f14] = new Register_Descriptor(f14, "f14", float_num, float_reg);
	spim_register_table[f16] = new Register_Descriptor(f16, "f16", float_num, float_reg);
	spim_register_table[f18] = new Register_Descriptor(f18, "f18", float_num, float_reg);
	spim_register_table[f20] = new Register_Descriptor(f20, "f20", float_num, float_reg);
	spim_register_table[f22] = new Register_Descriptor(f22, "f22", float_num, float_reg);
	spim_register_table[f24] = new Register_Descriptor(f24, "f24", float_num, float_reg);
	spim_register_table[f26] = new Register_Descriptor(f26, "f26", float_num, float_reg);
	spim_register_table[f28] = new Register_Descriptor(f28, "f28", float_num, float_reg);
	spim_register_table[f30] = new Register_Descriptor(f30, "f30", float_num, float_reg);
	spim_register_table[gp] = new Register_Descriptor(gp, "gp", int_num, pointer);
	spim_register_table[sp] = new Register_Descriptor(sp, "sp", int_num, pointer);
	spim_register_table[fp] = new Register_Descriptor(fp, "fp", int_num, pointer);
	spim_register_table[ra] = new Register_Descriptor(ra, "ra", int_num, ret_address);
}

void Machine_Description::initialize_instruction_table()
{
	spim_instruction_table[store] = new Instruction_Descriptor(store, "store", "sw", "", i_r_op_o1, a_op_o1_r);
	spim_instruction_table[load] = new Instruction_Descriptor(load, "load", "lw", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[la] = new Instruction_Descriptor(la, "loadAdd", "la", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[imm_load] = new Instruction_Descriptor(imm_load, "iLoad", "li", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[and_t] = new Instruction_Descriptor(and_t, "and", "and", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[or_t] = new Instruction_Descriptor(or_t, "or", "or", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[not_t] = new Instruction_Descriptor(not_t, "not", "sltu", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[mov] = new Instruction_Descriptor(mov, "move", "move", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[add] = new Instruction_Descriptor(add, "add", "add", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[sub] = new Instruction_Descriptor(sub, "sub", "sub", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[mult] = new Instruction_Descriptor(mult, "mul", "mul", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[divd] = new Instruction_Descriptor(divd, "div", "div", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[imm_add] = new Instruction_Descriptor(imm_add, "addi", "addi", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[uminus] = new Instruction_Descriptor(uminus, "uminus", "neg", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[store_d] = new Instruction_Descriptor(store_d, "store.d", "s.d", "", i_r_op_o1, a_op_o1_r);
	spim_instruction_table[load_d] = new Instruction_Descriptor(load_d, "load.d", "l.d", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[imm_load_d] = new Instruction_Descriptor(imm_load_d, "iLoad.d", "li.d", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[move_d] = new Instruction_Descriptor(move_d, "move.d", "mov.d", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[add_d] = new Instruction_Descriptor(add_d, "add.d", "add.d", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[sub_d] = new Instruction_Descriptor(sub_d, "sub.d", "sub.d", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[mult_d] = new Instruction_Descriptor(mult_d, "mul.d", "mul.d", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[div_d] = new Instruction_Descriptor(div_d, "div.d", "div.d", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[uminus_d] = new Instruction_Descriptor(uminus_d, "uminus.d", "neg.d", "", i_r_op_o1, a_op_r_o1);
	spim_instruction_table[slt] = new Instruction_Descriptor(slt, "slt", "slt", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[sle] = new Instruction_Descriptor(sle, "sle", "sle", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[sgt] = new Instruction_Descriptor(sgt, "sgt", "sgt", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[sge] = new Instruction_Descriptor(sge, "sge", "sge", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[seq] = new Instruction_Descriptor(seq, "seq", "seq", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[sne] = new Instruction_Descriptor(sne, "sne", "sne", "", i_r_o1_op_o2, a_op_r_o1_o2);
	spim_instruction_table[beq] = new Instruction_Descriptor(beq, "beq", "beq", "", i_op_o1_o2_st, a_op_o1_o2_st);
	spim_instruction_table[bne] = new Instruction_Descriptor(bne, "bne", "bne", "", i_op_o1_o2_st, a_op_o1_o2_st);
	spim_instruction_table[bgtz] = new Instruction_Descriptor(bgtz, "bgtz", "bgtz", "", i_op_o1_o2_st, a_op_o1_o2_st);
	spim_instruction_table[bgez] = new Instruction_Descriptor(bgez, "bgez", "bgez", "", i_op_o1_o2_st, a_op_o1_o2_st);
	spim_instruction_table[bltz] = new Instruction_Descriptor(bltz, "bltz", "bltz", "", i_op_o1_o2_st, a_op_o1_o2_st);
	spim_instruction_table[blez] = new Instruction_Descriptor(blez, "blez", "blez", "", i_op_o1_o2_st, a_op_o1_o2_st);
	spim_instruction_table[j] = new Instruction_Descriptor(j, "jump", "j", "", i_op_st, a_op_st);
	spim_instruction_table[jal] = new Instruction_Descriptor(jal, "jal", "jal", "", i_op_st, a_op_st);
	spim_instruction_table[print] = new Instruction_Descriptor(print, "print", "syscall", "", i_op, a_op);
	spim_instruction_table[label] = new Instruction_Descriptor(label, "", "", "", i_op_st, a_op_st);
	spim_instruction_table[ret_inst] = new Instruction_Descriptor(ret_inst, "return", "", "", i_op, a_op);
	spim_instruction_table[seq_d] = new Instruction_Descriptor(seq_d, "seq.d", "c.eq.d", "", i_o1_op_o2, a_op_o1_o2);
     spim_instruction_table[slt_d] = new Instruction_Descriptor(slt_d, "slt.d", "c.lt.d", "", i_o1_op_o2, a_op_o1_o2);
	spim_instruction_table[sle_d] = new Instruction_Descriptor(sle_d, "sle.d", "c.le.d", "", i_o1_op_o2, a_op_o1_o2);
	spim_instruction_table[sgt_d] = new Instruction_Descriptor(sgt_d, "sgt.d", "c.le.d", "", i_o1_op_o2, a_op_o1_o2);
	spim_instruction_table[sge_d] = new Instruction_Descriptor(sge_d, "sge.d", "c.lt.d", "", i_o1_op_o2, a_op_o1_o2);
	spim_instruction_table[sne_d] = new Instruction_Descriptor(sne_d, "sne.d", "c.eq.d", "", i_o1_op_o2, a_op_o1_o2);
	spim_instruction_table[bc1t] = new Instruction_Descriptor(bc1t, "bc1t", "bc1t", "", i_op_st, a_op_st);
	spim_instruction_table[bc1f] = new Instruction_Descriptor(bc1f, "bc1f", "bc1f", "", i_op_st, a_op_st);
}

void Machine_Description::validate_init_local_register_mapping_before_fn_call()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * reg_desc = i->second;

		if (reg_desc->is_used_for_fn_return() == false)
		{
			if (reg_desc->get_use_category() == int_reg)
			{
				CHECK_INVARIANT((reg_desc->is_free<int_reg>()), "GP data registers should be free at the start of a basic block or after a function call");
			}

			else if (reg_desc->get_use_category() == float_reg)
			{
				CHECK_INVARIANT((reg_desc->is_free<float_reg>()), "Float data registers should be free at the start of a basic block or after a function call");
			}
		}
	}
}

void Machine_Description::validate_init_local_register_mapping()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * reg_desc = i->second;

		if (reg_desc->get_use_category() == int_reg)
		{
			CHECK_INVARIANT((reg_desc->is_free<int_reg>()), "GP data registers should be free at the start of a basic block or after a function call");
		}

		else if (reg_desc->get_use_category() == float_reg)
		{
			CHECK_INVARIANT((reg_desc->is_free<float_reg>()), "Float data registers should be free at the start of a basic block or after a function call");
		}
	}
}

void Machine_Description::clear_local_register_mappings()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * reg_desc = i->second;
		reg_desc->clear_lra_symbol_list();
	}

	/* 
	Note that we do not need to save values at the end
	of a basic block because the values have already been
	saved for each assignment statement. Any optimization
	that tries to postpone the store statements may have to 
	consider storing all unstored values at the end of
	a basic block.
	*/
}

template <Register_Use_Category dt>
Register_Descriptor * Machine_Description::get_new_register()
{
	Register_Descriptor * reg_desc;
	int count = 0;

	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		reg_desc = i->second;

		CHECK_INVARIANT((reg_desc != NULL), "Null register descriptor in the register table");

		if (reg_desc->is_free<dt>()) 
		{       
			reg_desc->set_register_occupied();
			return reg_desc;
		}
	}

	machine_desc_object.clear_reg_not_used_for_expr_result();

	count = machine_desc_object.count_free_register<dt>();
	if (count > 0)
		return get_new_register<dt>();
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
			"Error in get_new_reg or register requirements of input program cannot be met");
	}
}

template <Register_Use_Category dt>
int Machine_Description::count_free_register()
{
	Register_Descriptor * rdp = NULL;
	int count = 0;

	map<Spim_Register, Register_Descriptor *>::iterator i;
	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		rdp = i->second;
		if (rdp->is_free<dt>())
			count++;
	}

	return count;
}

void Machine_Description::clear_reg_not_used_for_expr_result()
{
	map<Spim_Register, Register_Descriptor *>::iterator i;

	for (i = spim_register_table.begin(); i != spim_register_table.end(); i++)
	{
		Register_Descriptor * rdp = i->second;

		if(!rdp->is_used_for_expr_result()) 
		{
			rdp->reset_register_occupied();  /* reset reg occupied i.e register is not anymore occupied */
			rdp->clear_lra_symbol_list();
			break;
		}
	}
}

template bool Register_Descriptor::is_free<int_reg>();
template bool Register_Descriptor::is_free<float_reg>();

template void Lra_Outcome::optimize_lra<int_reg>(Lra_Scenario lcase, Ast * destination_memory, Ast * source_memory);
template void Lra_Outcome::optimize_lra<float_reg>(Lra_Scenario lcase, Ast * destination_memory, Ast * source_memory);

template Register_Descriptor * Machine_Description::get_new_register<int_reg>();
template Register_Descriptor * Machine_Description::get_new_register<float_reg>();

template int Machine_Description::count_free_register<int_reg>();
template int Machine_Description::count_free_register<float_reg>();
