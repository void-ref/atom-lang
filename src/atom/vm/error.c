/**
 * File:   error.c
 * Author: Mitchell Matsumori-Kelly
 * Date:   11/27/2018
 */

#include "atom/vm/error.h"
#include "atom/vm/vm.h"

#include <stdio.h>


/**
 * Throws an error on the given vm
 * @param error The error to throw
 * @param vm    The VM
 * @return      ATOM_TRUE if execution should continue
 */
atom_bool at_error_throw(const atError *error, atVM *vm) {

	// print the error
	if ( vm->debug || vm->executor.strict )
		printf("%s: %s", vm->executor.strict ? "Fatal error" : "Error", error->message);
	
	vm->last_error = *error;

	return !vm->executor.strict;
}
