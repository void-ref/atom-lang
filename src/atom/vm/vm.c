/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: vm.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#include "atom/vm/vm.h"
#include "atom/vm/libs/libs.h"

#include <stdlib.h>

/**
 * Initializes a virtual machine.
 * @param  vm The VM
 * @return    The VM
 */
atVM* at_vm_init(atVM *vm) {

	// initialize the global table
	at_table_init(vm, &vm->global_table);

	// initialize the executor
	at_executor_init(vm, &vm->executor);

	// load libraries
	at_lib_load(vm, ATOM_LIB_ALL);

	return vm;
}


/**
 * Frees all resources from a virtual machine, allowing it to be safely deleted.
 * @param vm The VM
 */
void at_vm_terminate(atVM *vm) {

	// free the global table
	at_table_free(&vm->global_table);

	// free the executor
	at_executor_free(&vm->executor);

}


/**
 * Allocates a block of memory on a VM
 * @param vm   The VM
 * @param size The size of the block to allocate
 * @return     A pointer to the memory block or NULL if allocation failed
 */
void* at_vm_alloc(atVM *vm, unsigned size) {
	return malloc(size); // just use malloc() for now
}

/**
 * Frees a block of memory from a VM
 * @param vm  The VM
 * @param mem The memory block
 */
void at_vm_free(atVM *vm, void *mem) {
	free(mem); // just use free() for now
}






/**
 * Returns the top value on the operand stack of the
 * top frame
 * @param  vm The VM
 * @return    The top value
 */
atValue* at_vm_top(atVM *vm) {
	atCallFrame *frame = at_executor_top_frame(&vm->executor);
	return at_frame_top(frame);
}

/**
 * Pushes a value onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The value
 */
void at_vm_push(atVM *vm, atValue *value) {
	atCallFrame *frame = at_executor_top_frame(&vm->executor);
	at_frame_push(frame, value);
}

/**
 * Pops the top value from the operand stack of the
 * top frame
 * @param vm The VM
 */
void at_vm_pop(atVM *vm) {
	atCallFrame *frame = at_executor_top_frame(&vm->executor);
	at_frame_pop(frame);
}

/**
 * Pushes a null value onto the operand stack of the
 * top frame
 * @param vm    The VM
 */
void at_vm_push_null(atVM *vm) {
	atValue new_value;
	at_value_set_null(&new_value);
	at_vm_push(vm, &new_value);
}


/**
 * Pushes a number onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The number
 */
void at_vm_push_number(atVM *vm, atom_number value) {
	atValue new_value;
	at_value_from_number(&new_value, value);
	at_vm_push(vm, &new_value);
}

/**
 * Pushes a string onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The string
 */
void at_vm_push_string(atVM *vm, const char *value) {
	atValue new_value;
	at_value_from_string(&new_value, value);
	at_vm_push(vm, &new_value);
}

/**
 * Pushes a user function onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The user function
 */
void at_vm_push_cfunction(atVM *vm, atUserFunction value) {
	atValue new_value;
	at_value_from_cfunction(&new_value, value);
	at_vm_push(vm, &new_value);
}

/**
 * Pushes a function onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The function
 */
void at_vm_push_function(atVM *vm, atFunction *value) {
	atValue new_value;
	at_value_from_function(&new_value, value);
	at_vm_push(vm, &new_value);
}

/**
 * Pushes a table onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The table
 */
void at_vm_push_table(atVM *vm, atTable *value) {
	atValue new_value;
	at_value_from_table(&new_value, value);
	at_vm_push(vm, &new_value);
}

/**
 * Stores a value from the operand stack with
 * specified key in the current call frame
 * @param vm  The VM
 * @param key The key
 */
void at_vm_store(atVM *vm, const char *key) {
	atCallFrame *frame = at_executor_top_frame(&vm->executor);
	at_frame_store(frame, key);
}


/**
 * Pops a number from the operand stack of the
 * top frame
 * @param  vm  The VM
 * @param  out Where to store the number
 * @return     TRUE if the value popped was a number, FALSE otherwise
 */
atom_bool at_vm_pop_number(atVM *vm, atom_number *out) {
	atValue *top = at_vm_top(vm);

	if ( top == NULL || top->type != ATOM_VALUE_NUMBER ) {
		*out = 0;
		at_vm_pop(vm);
		return ATOM_FALSE;
	}


	*out = at_value_to_number(top);
	at_vm_pop(vm);
	return ATOM_TRUE;
}
