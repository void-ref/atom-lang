/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/27/2018
 * @Filename: error.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_VM_ERROR_H
#define ATOM_VM_ERROR_H


#include "atom/types.h"


typedef struct _atError {
	int debug_row;
	int debug_column;

	char *message;
} atError;


/**
 * Throws an error on the given vm
 * @param error The error to throw
 * @param vm    The VM
 * @return      ATOM_TRUE if execution should continue
 */
atom_bool at_error_throw(const atError *error, atVM *vm);





#endif
