/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: vm.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_VM_VM_H
#define ATOM_VM_VM_H

#include "atom/vm/executor.h"
#include "atom/vm/table.h"
#include "atom/vm/error.h"


typedef struct _atVM {
	atTable global_table;
	atExecutor executor;


	// If TRUE, debug messages are printed
	atom_bool debug;
	atError last_error;
} atVM;


/**
 * Initializes a virtual machine.
 * @param  vm The VM
 * @return    The VM
 */
atVM* at_vm_init(atVM *vm);


/**
 * Frees all resources from a virtual machine, allowing it to be safely deleted.
 * @param vm The VM
 */
void at_vm_terminate(atVM *vm);


/**
 * Allocates a block of memory on a VM
 * @param vm   The VM
 * @param size The size of the block to allocate
 * @return     A pointer to the memory block or NULL if allocation failed
 */
void* at_vm_alloc(atVM *vm, unsigned size);

/**
 * Frees a block of memory from a VM
 * @param vm  The VM
 * @param mem The memory block
 */
void at_vm_free(atVM *vm, void *mem);



/**
 * Returns the top value on the operand stack of the
 * top frame
 * @param  vm The VM
 * @return    The top value
 */
atValue* at_vm_top(atVM *vm);

/**
 * Pushes a value onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The value
 */
void at_vm_push(atVM *vm, atValue *value);

/**
 * Pops the top value from the operand stack of the
 * top frame
 * @param vm The VM
 */
void at_vm_pop(atVM *vm);


/**
 * Pushes a null value onto the operand stack of the
 * top frame
 * @param vm    The VM
 */
void at_vm_push_null(atVM *vm);

/**
 * Pushes a number onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The number
 */
void at_vm_push_number(atVM *vm, atom_number value);

/**
 * Pushes a string onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The string
 */
void at_vm_push_string(atVM *vm, const char *value);

/**
 * Pushes a user function onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The user function
 */
void at_vm_push_cfunction(atVM *vm, atUserFunction value);

/**
 * Pushes a function onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The function
 */
void at_vm_push_function(atVM *vm, atFunction *value);

/**
 * Pushes a table onto the operand stack of the
 * top frame
 * @param vm    The VM
 * @param value The table
 */
void at_vm_push_table(atVM *vm, atTable *value);

/**
 * Stores a value from the operand stack with
 * specified key in the current call frame
 * @param vm  The VM
 * @param key The key
 */
void at_vm_store(atVM *vm, const char *key);





/**
 * Pops a number from the operand stack of the
 * top frame
 * @param  vm  The VM
 * @param  out Where to store the number
 * @return     TRUE if the value popped was a number, FALSE otherwise
 */
atom_bool at_vm_pop_number(atVM *vm, atom_number *out);






#endif
