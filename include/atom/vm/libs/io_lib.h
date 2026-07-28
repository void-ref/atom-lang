/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/22/2018
 * @Filename: io_lib.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_VM_LIBS_IO_LIB_H
#define ATOM_VM_LIBS_IO_LIB_H


typedef struct _atVM atVM;


/**
 * Loads the IO library onto the VM
 * @param vm The VM
 */
void at_lib_load_io(atVM *vm);


#endif
