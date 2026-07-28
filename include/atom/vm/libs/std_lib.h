/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/26/2018
 * @Filename: std_lib.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */




 #ifndef ATOM_VM_LIBS_STD_LIB_H
 #define ATOM_VM_LIBS_STD_LIB_H


 typedef struct _atVM atVM;


 /**
  * Loads the standard library onto the VM
  * @param vm The VM
  */
 void at_lib_load_std(atVM *vm);


 #endif
