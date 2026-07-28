/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/22/2018
 * @Filename: math_lib.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */



#ifndef ATOM_VM_LIBS_MATH_LIB_H
#define ATOM_VM_LIBS_MATH_LIB_H




typedef struct _atVM atVM;



/**
 * Loads the math library onto the VM
 * @param vm The VM
 */
void at_lib_load_math(atVM *vm);





#endif
