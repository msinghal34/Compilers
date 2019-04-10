
#ifndef REG_ALLOC_HH
#define REG_ALLOC_HH

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

/////////////////////////////// Register_Descriptor ////////////////////

typedef enum
{
	none,	/* dummy to indicate no register */
	zero,	/* constant register */
	v0,	/* expression result register */
	v1,	/* function result register */
	a0,	/* argument register */
	a1, a2, a3,
	t0,      /* temporary caller-save registers */
	t1, t2, t3, t4, t5, t6, t7, t8, t9, 
	s0,	/* temporary callee-save registers */ 
	s1, s2, s3, s4, s5, s6, s7,
	mfc,	/* float register to int register */
	mtc,	/* int register to float register */
	f0, 	/* floating point registers */
	f2, f4, f6, f8,
	f10, f12, f14, f16, f18,
	f20, f22, f24, f26, f28, f30,
	gp,	/* global data pointer register */
	sp,	/* stack pointer register */
	fp,	/* frame pointer register */
	ra	/* return address register */
} Spim_Register;

typedef enum 
{ 
	int_num,
	float_num
} Register_Val_Type;

typedef enum 
{ 
	fixed_reg, 
	int_reg, 
	fn_result, 
	argument,
	pointer, 
	ret_address,
	float_reg
} Register_Use_Category;

class Register_Descriptor
{
    Spim_Register reg_id;
    string reg_name;
    Register_Val_Type value_type;
    Register_Use_Category reg_use; 

    list<Symbol_Table_Entry *> lra_symbol_list;
    bool used_for_expr_result;
    bool reg_occupied;
    bool used_for_fn_return;

  public:
    Register_Descriptor (Spim_Register reg, string nam, Register_Val_Type vt, Register_Use_Category uc);
    Register_Descriptor() {}
    ~Register_Descriptor() {}

    bool is_symbol_list_empty();
    void update_symbol_information(Symbol_Table_Entry & symbol_entry);

    int count_symbol_entry_in_list();

    bool find_symbol_entry_in_list(Symbol_Table_Entry & symbol_entry);
    void remove_symbol_entry_from_list(Symbol_Table_Entry & symbol_entry);

    Register_Use_Category get_use_category(); 
    Spim_Register get_register();

    string get_name();
    void clear_lra_symbol_list();

    bool is_register_occupied();
    void set_register_occupied();
    void reset_register_occupied();

    bool is_used_for_fn_return();
    void set_used_for_fn_return();
    void reset_used_for_fn_return();

    bool is_used_for_expr_result();
    void set_use_for_expr_result();
    void reset_use_for_expr_result();

    template <Register_Use_Category dt>
    bool is_free();
};

////////////////////////////// Lra_Outcome ///////////////////////////////////////

/*   
    The local register allocation (lra) restricts its attention to a basic block
    and maintains the following two pieces of information

    - for each name encountered, the register in which current value of name
      can be found 
    - for each register, the set of names whose values are held in the
      register (they all have the same value).

    At the start of a block each name has register "none" and the name list of
    each register is empty.
     Local register allocation should consider the following cases described 
     in terms of "source to destination" moves where source and destination
     could be memory (m), register (r), or constant (c).

     For each case we outline a step-wise decision process for identifying
     (a) The register, (b) Load, if any

     1. m2m is a typical assginment statemenmt involving memory on lhs
        as well as RHS. Decision steps
               if (rhs is in a register)
               {
                   use the register. 
                   no load required.
               }
               else if (lhs is in a register)
               {
                   use the register.
                   load rhs into the register.
               }
               else
               {
                   use a new register.
                   load rhs into the register.
               }
     2. m2r is a typical load for a compute statemenmt. As a policy
        we use a new register because the value may be updated by the
        computation. This can be minimized by traversing the tree using
        Sethi-Ullman lablelling, but we do not do it in this version.
               if (rhs is in a register)
               {
                   use a new register. 
                   move from source register into the new register.
               }
               else
               {
                   use a new register.
                   load rhs into the register.
               }
    3. r2m. This is a typical store for a compute statemenmt. It does not need a 
       new register 
    4. c2m. This is a store that needs an intermediate register.
               if (lhs is in a register)
               {
                   use the register. 
                   load the rhs constant into the register.
               }
               else
               {
                   use a new register.
                   load rhs constant into the new register.
               }
    5. c2r. This is typical load immediate for computation. 
               unconditionally
               {
                   use a new register.
                   load rhs constant into the new register.
               } 
    6. r2r does not need to bring in register allocation.

    Given the 3 candidates (m,r,c) and 2 positions (source and destination)
    we have 3^2 = 9 possibility. Apart from the 6 possibilities
    outlined above, the remaining three possibilities (m2c, r2c, c2c)
    do not arise on common architectures.


    In each of the above case, the following side-effects are noted:
    - the register of the destination mem location is updated to 
      the new register by removing the earlier register.
    - the destination is added to the name list of the new register. 

    An invariant of our policy is that for a given location there 
    is only one register, although for a given register, there may be 
    multiple locations. This happens because of a call to register 
    allocation 
    - NEVER adds a source location to any register, and
    - overwrites the previous register of the destination of the 
      new register.


    At the end of a block both these information are nullified by setting
    register to "none" for each name and by setting the name list of each 
    register to empty.
*/

typedef enum 
{ 
	mc_2m = 1, 
	mc_2r, 
	r2m, 
	c2m,
	r2r
} Lra_Scenario;

class Lra_Outcome
{
    Register_Descriptor * register_description;

    bool is_a_new_register;
    bool is_same_as_source;
    bool is_same_as_destination;
    bool register_move_needed; 
    bool load_needed;

  public:
    Lra_Outcome(Register_Descriptor * rd, bool nr, bool sr, bool dr, bool mv, bool ld);
    Lra_Outcome();
    ~Lra_Outcome() {}
    
    Register_Descriptor * get_register();
    bool is_new_register();
    bool is_source_register();
    bool is_destination_register();
    bool is_move_needed();
    bool is_load_needed();

    template <Register_Use_Category dt>
    void optimize_lra(Lra_Scenario lcase, Ast * destination_memory, Ast * source_memory);
};

///////////////////////////// Machine Description ////////////////////////////////

class Machine_Description
{
public:
	map<Tgt_Op, Instruction_Descriptor *> spim_instruction_table;
	map<Spim_Register, Register_Descriptor *> spim_register_table;

	void initialize_instruction_table();
	void initialize_register_table();

	void validate_init_local_register_mapping_before_fn_call();
	void validate_init_local_register_mapping();
	void clear_local_register_mappings();

	void clear_reg_not_used_for_expr_result();

	template <Register_Use_Category dt>
	int count_free_register();

	template <Register_Use_Category dt>
	Register_Descriptor * get_new_register();
};

extern Machine_Description machine_desc_object;

#endif
