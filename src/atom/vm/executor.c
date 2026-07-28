/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: executor.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/06/2018
 */

#include "atom/vm/executor.h"
#include "atom/vm/frame.h"
#include "atom/vm/table.h"
#include "atom/vm/bytecode.h"
#include "atom/vm/function.h"
#include "atom/vm/executor_funcs.h"
#include "atom/parser/parser.h"
#include "atom/util/memory_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define ATOM_EXECUTOR_INITIAL_CALL_FRAME_CAPACITY 16







/**
 * Pushes the global frame onto the executor
 * @param  exe The executor
 * @return     The global frame
 */
atCallFrame* _at_executor_push_global_frame(atExecutor *exe);




/**
 * Initializes an executor for the given VM
 * @param  vm       The VM
 * @param  executor The executor
 * @return          The executor
 */
atExecutor* at_executor_init(atVM *vm, atExecutor *executor) {

	// set some fields
	executor->vm     = vm;
	executor->strict = ATOM_TRUE;

	// allocate the call stack
	executor->call_stack = malloc(sizeof(atCallFrame) * ATOM_EXECUTOR_INITIAL_CALL_FRAME_CAPACITY);
	executor->call_stack_capacity = ATOM_EXECUTOR_INITIAL_CALL_FRAME_CAPACITY;
	executor->call_stack_size = 0;


	// Load the global frame
	at_executor_push_frame(executor);


	return executor;
}

/**
 * Frees the memory associated with an executor
 * @param executor The executor
 */
void at_executor_free(atExecutor *executor) {

	// The call stack should be empty here, but we'll try clearing
	// it anyway
	if ( executor->call_stack_size > 1 ) {
		printf("Warning: freeing executor with %d call frames left.\n", executor->call_stack_size);
		while ( executor->call_stack_size != 1 )
			at_executor_pop_frame(executor);

		//at_frame_free(&executor->call_stack[0]);
	}
}

/**
 * Pushes a new frame onto the call stack
 * @param  executor The executor
 * @return          The call frame
 */
atCallFrame* at_executor_push_frame(atExecutor *executor) {

	// check if the call stack needs to be reallocated
	if ( executor->call_stack_size >= executor->call_stack_capacity ) {
		unsigned new_capacity = executor->call_stack_capacity * 2;
		atCallFrame *new_stack =  at_array_copy(executor->call_stack, executor->call_stack_size, new_capacity);
		free(executor->call_stack);
		executor->call_stack = new_stack;
	}

	// Initialize a new call frame
	atCallFrame *frame = executor->call_stack + executor->call_stack_size;
	at_frame_init(executor->vm, frame, 64);
	++executor->call_stack_size;


	return frame;
}


/**
 * Pops a frame from the call stack
 * @param executor The executor
 */
void at_executor_pop_frame(atExecutor *executor) {

	// Note: we cannot pop the first element in the call stack
	// since it is the global frame
	if ( executor->call_stack_size > 1 ) {
		// free the frame
		at_frame_free(executor->call_stack + --executor->call_stack_size);
	}
	else
		printf("Warning: attempted to pop empty call stack.\n");
}

/**
 * Returns the top frame of the call stack
 * @param executor The executor
 * @return         The top frame
 */
atCallFrame* at_executor_top_frame(atExecutor *executor) {
	if ( executor->call_stack_size == 0 )
		return NULL;
	else
		return &executor->call_stack[executor->call_stack_size - 1];
}

/**
 * Executes the given source code
 * @param exe  The executor
 * @param code The source code
 */
void at_executor_run_source(atExecutor *exe, const char *code) {

	// create a new parser
	atParser parser;
	at_parser_init(&parser, code);


	// run the parser to generate bytecode
	at_parser_parse(&parser);

	// make sure the code is error-free
	if ( parser.error_count == 0 ) {

		// print the bytecode that was parsed
		// printf("Bytecode length: %d\n", parser.bytecode_length);
		at_bc_print(parser.bytecode, parser.bytecode_length);

		// run the executor
		at_executor_run_bytecode(exe, parser.bytecode, parser.bytecode_length);
	}

	// free the parser
	at_parser_free(&parser);
}

/**
 * Executes the given bytecode from the global context
 * @param exe  The executor
 * @param code The bytecode
 * @param length The bytecode length
 */
void at_executor_run_bytecode(atExecutor *exe, const atom_byte code[], unsigned length) {
	atom_byte *p    = code;
	atom_byte *end  = p + length;

	atVM *vm = exe->vm;

	// execute instructions until the end of the program is reached
	while ( ( p = _at_executor_do_next_instruction(vm, p, end)) != end && p != NULL );

	return;
}
