/**
 * @Author: Mitchell Kelly <mitchell>
 * @Date:   11/23/2018
 * @Filename: executor_funcs.h
 * @Last modified by:   mitchell
 * @Last modified time: 12/06/2018
 *
 * Contains prototypes for functions that execute individual
 * bytecode instructions
 */

#ifndef ATOM_VM_EXECUTOR_FUNCS_H
#define ATOM_VM_EXECUTOR_FUNCS_H

#include "atom/types.h"


typedef struct _atExecutor atExecutor;
typedef struct _atValue atValue;
typedef struct _atVM atVM;

typedef atom_byte* (*atExecutorFunction)(atVM *vm, atom_byte *start, atom_byte *end);


/**
 * Executes the next instruction in the given bytecode array
 * @param  vm    The VM to run the bytecode on
 * @param  start A pointer to the starting instruction
 * @param  end   A pointer to the end of the bytecode array
 * @return       A pointer to the new position in the bytecode array
 */
atom_byte* _at_executor_do_next_instruction(atVM *vm, atom_byte *start, atom_byte *end);


/**
 * Halts execution
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_halt(atVM *vm, atom_byte *p, atom_byte *end);


/**
 * Performs the push_null operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_push_null(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the push_number operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_push_number(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the push_string operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_push_string(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the load_value operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_load_value(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the store operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_store(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the store_local operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_store_local(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the table store operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_store_table(atVM *vm, atom_byte *p, atom_byte *end);


/**
 * Performs the index operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_index(atVM *vm, atom_byte *p, atom_byte *end);






/**
 * Performs the new_function operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_new_function(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the call operation
 * @param  exe   The executor
 * @param  start The start pointer
 * @param  end   The end pointer
 * @return       The new position
 */
atom_byte* _at_executor_do_call(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the argument load operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_load_argument(atVM *vm, atom_byte *p, atom_byte *end);





/**
 * Performs the jump operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_jump(atVM *vm, atom_byte *p, atom_byte *end);

/**
 * Performs the conditional jump operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_cjump(atVM *vm, atom_byte *p, atom_byte *end);


// arithmetic
atom_byte* _at_executor_do_add(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_sub(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_mul(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_div(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_mod(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_exp(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_neg(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_fact(atVM *vm, atom_byte *p, atom_byte *end);

// comparison
atom_byte* _at_executor_do_eq(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_neq(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_gt(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_gte(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_lt(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_lte(atVM *vm, atom_byte *p, atom_byte *end);

// boolean
atom_byte* _at_executor_do_not(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_and(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_or(atVM *vm, atom_byte *p, atom_byte *end);
atom_byte* _at_executor_do_xor(atVM *vm, atom_byte *p, atom_byte *end);




#endif
