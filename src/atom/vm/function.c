/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: function.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#include "atom/vm/function.h"

#include <stdlib.h>
#include <memory.h>

/**
 * Initializes a function
 * @param  func      The function
 * @param  bc        The bytecode for the function
 * @param  length    The length
 * @param  arg_names The names of the arguments (in order)
 * @param  num_args  The number of arguments to the function
 * @return           The function
 */
atFunction* at_function_init(atFunction *func, atom_byte *bc, unsigned length) {

	// copy the bytecode array
	func->bytecode_length = length;
	func->bytecode        = malloc(length);
	memcpy(func->bytecode, bc, length);

	return func;
}

/**
 * Frees the memory associated with an atFunction
 * @param func The function
 */
void at_function_free(atFunction *func) {

	// free the bytecode array
	free(func->bytecode);
	func->bytecode        = NULL;
	func->bytecode_length = 0;

}
