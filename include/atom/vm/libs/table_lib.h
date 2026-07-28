/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/27/2018
 * @Filename: table_lib.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 *
 * Contains the prototype for the function that loads
 * the atom table library onto a VM
 */



#ifndef ATOM_VM_LIBS_TABLE_LIB_H
#define ATOM_VM_LIBS_TABLE_LIB_H



typedef struct _atVM atVM;


/**
 * Loads the IO library onto the VM
 * @param vm The VM
 */
void at_lib_load_table(atVM *vm);


#endif
