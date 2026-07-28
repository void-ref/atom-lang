/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/22/2018
 * @Filename: libs.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_VM_LIBS_LIBS_H
#define ATOM_VM_LIBS_LIBS_H


#include "atom/vm/libs/std_lib.h"
#include "atom/vm/libs/math_lib.h"
#include "atom/vm/libs/io_lib.h"
#include "atom/vm/libs/table_lib.h"


typedef enum _atLibrary {
	ATOM_LIB_ALL   = 0,
	ATOM_LIB_STD   = 1 << 0,
	ATOM_LIB_MATH  = 1 << 1,
	ATOM_LIB_IO    = 1 << 2,
	ATOM_LIB_TABLE = 1 << 3,
} atLibrary;

/**
 * Loads a library (or multiple) onto a VM
 * @param vm  The VM
 * @param lib A bitwise combination of atLibrary's. Set to zero for all
 */
void at_lib_load(atVM *vm, atLibrary lib);



#endif
