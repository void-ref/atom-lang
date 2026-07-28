/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: function.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_VM_FUNCTION_H
#define ATOM_VM_FUNCTION_H

#include "atom/types.h"

typedef struct _atVM atVM;



typedef struct _atFunction {
	atom_byte *bytecode;
	unsigned   bytecode_length;
} atFunction;

/**
 * Initializes a function
 * @param  func      The function
 * @param  bc        The bytecode for the function
 * @param  length    The length
 * @return           The function
 */
atFunction* at_function_init(atFunction *func, atom_byte *bc, unsigned length);

/**
 * Frees the memory associated with an atFunction
 * @param func The function
 */
void at_function_free(atFunction *func);


#endif
