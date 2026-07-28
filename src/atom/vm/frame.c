/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: frame.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#include "atom/vm/frame.h"
#include "atom/vm/value.h"
#include "atom/util/memory_util.h"

#include <stdio.h>
#include <stdlib.h>



/**
 * Initializes a call frame
 * @param  vm               The VM
 * @param  frame            The call frame
 * @param  operand_capacity The initial operand stack capacity
 * @return                  The
 */
atCallFrame* at_frame_init(atVM *vm, atCallFrame *frame, unsigned operand_capacity) {

	frame->vm = vm;

	// initialize the stack table
	at_table_init(vm, &frame->stack);

	// allocate the operands
	frame->operands = malloc(sizeof(atValue) * operand_capacity);
	frame->operand_capacity = operand_capacity;
	frame->operand_top = 0;

	return frame;
}

/**
 * De-initialies a call frame
 * @param frame The call frame
 */
void at_frame_free(atCallFrame *frame) {

	// Free the operands. There should be none left,
	// but we'll try to pop them just in case
	if ( frame->operand_top != 0 ) {
		printf("warning: freeing call frame with %d operands left.\n", frame->operand_top);

		while ( frame->operand_top != 0 )
			at_frame_pop(frame);
	}

	free(frame->operands);
	frame->operand_top = 0;
	frame->operand_capacity = 0;

	// free the stack memory
	at_table_free(&frame->stack);

	frame->vm = 0;
}




/**
 * Pushes a value from the call table onto the operand stack
 * @param frame The call frame
 * @param key   The key in the call table (i.e. variable name)
 */
void at_frame_load(atCallFrame *frame, const char *key) {

	// retrieve a value from the stack memory
	atValue *value = at_table_get(&frame->stack, key);

	// push the value onto the operand stack
	at_frame_push(frame, value);
}

/**
 * Stores the top value from the operand stack into the call table
 * @param frame The call frame
 * @param key   The key in the call table (i.e. variable name)
 */
void at_frame_store(atCallFrame *frame, const char *key) {

	// get the top from the operand stack
	atValue *to_store = at_frame_top(frame);

	// store the value in stack memory
	at_table_set(&frame->stack, key, to_store);

	// pop the operand
	at_frame_pop(frame);
}

/**
 * Pushes a value onto the operand stack
 * @param  frame The call frame
 * @param value The value
 */
void at_frame_push(atCallFrame *frame, atValue *value) {

	// check if the operand stack needs to be resized
	if ( frame->operand_top == frame->operand_capacity ) {
		unsigned new_capacity = frame->operand_capacity * 2;
		atValue *new_stack = at_array_copy(frame->operands, frame->operand_top, new_capacity);

		free(frame->operands);
		frame->operands = new_stack;
		frame->operand_capacity = new_capacity;
	}


	// Special case for null values
	if ( value == NULL || value->type == ATOM_VALUE_NULL ) {
		at_value_set_null(&frame->operands[frame->operand_top++]);
	}
	else {
		// store the value and reference it
		frame->operands[frame->operand_top++] = *value;
		at_value_reference(value);
	}

}

/**
 * Pops a value from the operand stack of a call frame
 * @param  frame The call frame
 */
void at_frame_pop(atCallFrame *frame) {

	if ( frame->operand_top == 0 ) {
		//printf("Warning: attempted to pop an empty frame\n");
		return;
	}

	// get the top element
	atValue *top = at_frame_top(frame);

	// dereference the top value
	at_value_dereference(top);

	// decrease the stack size
	--frame->operand_top;
}




/**
 * Returns the top of the operand stack of a frame
 * @param  frame The frame
 * @return       The value or NULL if empty
 */
atValue* at_frame_top(atCallFrame *frame) {

	// check if the stack is empty
	if ( frame->operand_top == 0 ) {
		return NULL;
	}

	// return the top value
	return frame->operands + frame->operand_top - 1;
}








/**
 * Pushes a number onto the operand stack of a call frame
 * @param  frame The call frame
 * @param  num   The number to push
 * @return       The index of the pushed value
 */
unsigned at_frame_push_number(atCallFrame *frame, atom_number num) {

	// create a new value from the number
	atValue value;
	at_value_from_number(&value, num);

	// push the value onto the operand stack
	at_frame_push(frame, &value);

	return frame->operand_top - 1;
}

/**
 * Pushes a string onto the operand stack of a call frame
 * @param  frame The call frame
 * @param  str   The string to push
 * @return       The index of the pushed value
 */
unsigned at_frame_push_string(atCallFrame *frame, const char *str) {
	// create a new value from the string
	atValue value;
	at_value_from_string(&value, str);

	// push the value onto the operand stack
	at_frame_push(frame, &value);

	return frame->operand_top - 1;
}


/**
 * Returns the size of the operand stack of a frame
 * @param  frame The frame
 * @return       The size of the stack
 */
unsigned at_frame_size(atCallFrame *frame) {
	return frame->operand_top;
}
