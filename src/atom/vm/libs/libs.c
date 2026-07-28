/**
 * File:   libs.c
 * Author: Mitchell Matsumori-Kelly
 * Date:   11/28/2018
 */

#include "atom/vm/libs/libs.h"
#include "atom/vm/vm.h"





/**
 * Loads a library (or multiple) onto a VM
 * @param vm  The VM
 * @param lib A bitwise combination of atLibrary's
 */
void at_lib_load(atVM *vm, atLibrary lib) {

	// set all bits if all libraries are requested
	if ( lib == ATOM_LIB_ALL )
		lib = ~0;

	// check which libraries should be loaded
	if ( lib & ATOM_LIB_STD )
		at_lib_load_std(vm);
	if ( lib & ATOM_LIB_MATH )
		at_lib_load_math(vm);
	if ( lib & ATOM_LIB_IO )
		at_lib_load_io(vm);
	if ( lib & ATOM_LIB_TABLE )
		at_lib_load_table(vm);
}
