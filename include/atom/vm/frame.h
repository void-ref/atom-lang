/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: frame.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#ifndef ATOM_VM_FRAME_H
#define ATOM_VM_FRAME_H

#include "atom/types.h"
#include "atom/vm/table.h"

typedef struct _atVM atVM;

typedef struct _atCallFrame {
	atVM     *vm;
	atTable   stack;
	atValue  *operands;
	unsigned  operand_top;
	unsigned  operand_capacity;
} atCallFrame;


/**
 * Initializes a call frame
 * @param  vm               The VM
 * @param  frame            The call frame
 * @param  operand_capacity The initial operand stack capacity
 * @return                  The
 */
atCallFrame* at_frame_init(atVM *vm, atCallFrame *frame, unsigned operand_capacity);

/**
 * De-initialies a call frame
 * @param frame The call frame
 */
void at_frame_free(atCallFrame *frame);




/**
 * Pushes a value from the call table onto the operand stack
 * @param frame The call frame
 * @param key   The key in the call table (i.e. variable name)
 */
void at_frame_load(atCallFrame *frame, const char *key);

/**
 * Stores the top value from the operand stack into the call table
 * @param frame The call frame
 * @param key   The key in the call table (i.e. variable name)
 */
void at_frame_store(atCallFrame *frame, const char *key);


/**
 * Pushes a number onto the operand stack of a call frame
 * @param  frame The call frame
 * @param  num   The number to push
 * @return       The index of the pushed value
 */
unsigned at_frame_push_number(atCallFrame *frame, atom_number num);

/**
 * Pushes a string onto the operand stack of a call frame
 * @param  frame The call frame
 * @param  str   The string to push
 * @return       The index of the pushed value
 */
unsigned at_frame_push_string(atCallFrame *frame, const char *str);


/**
 * Pushes a value onto the operand stack
 * @param  frame The call frame
 * @param value The value
 */
void at_frame_push(atCallFrame *frame, atValue *value);

/**
 * Pops a value from the operand stack of a call frame
 * @param  frame The call frame
 */
void at_frame_pop(atCallFrame *frame);


/**
 * Returns the top of the operand stack of a frame
 * @param  frame The frame
 * @return       The value or NULL if empty
 */
atValue* at_frame_top(atCallFrame *frame);

/**
 * Returns the size of the operand stack of a frame
 * @param  frame The frame
 * @return       The size of the stack
 */
unsigned at_frame_size(atCallFrame *frame);













#endif
