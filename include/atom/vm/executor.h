/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: executor.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/28/2018
 *
 * Contains prototypes for functions that perform operations
 * on an atExecutor which is also declared here.
 */

#ifndef ATOM_VM_EXECUTOR_H
#define ATOM_VM_EXECUTOR_H

#include "atom/types.h"
#include "atom/vm/frame.h"

/**
 * Contains all of the stack and operand memory for a VM.
 * Bytecode which is run on the executor will use this data.
 */
typedef struct _atExecutor {

	// the virtual machine this executor belongs to
	atVM *vm;

	// the call stack
	atCallFrame *call_stack;
	unsigned call_stack_capacity;
	unsigned call_stack_size;



	// If TRUE, execution halts upon all runtime errors
	// If FALSE, soft errors are thrown for most runtime errors.
	atom_bool strict;

} atExecutor;


/**
 * Initializes an executor for the given VM
 * @param  vm       The VM
 * @param  executor The executor
 * @return          The executor
 */
atExecutor* at_executor_init(atVM *vm, atExecutor *executor);

/**
 * Frees the memory associated with an executor
 * @param executor The executor
 */
void at_executor_free(atExecutor *executor);

/**
 * Pushes a new frame onto the call stack
 * @param  executor The executor
 * @return          The call frame
 */
atCallFrame* at_executor_push_frame(atExecutor *executor);


/**
 * Pops a frame from the call stack
 * @param executor The executor
 */
void at_executor_pop_frame(atExecutor *executor);

/**
 * Returns the top frame of the call stack
 * @param executor The executor
 * @return         The top frame
 */
atCallFrame* at_executor_top_frame(atExecutor *executor);



/**
 * Executes the given source code
 * @param exe  The executor
 * @param code The source code
 */
void at_executor_run_source(atExecutor *exe, const char *code);

/**
 * Executes the given bytecode from the global context
 * @param exe    The executor
 * @param code   The bytecode
 * @param length The bytecode length
 */
void at_executor_run_bytecode(atExecutor *exe, const atom_byte code[], unsigned length);



#endif
