/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/27/2018
 * @Filename: table_lib.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#include "atom/vm/libs/io_lib.h"
#include "atom/vm/vm.h"
#include "atom/util/memory_util.h"

#include <stdio.h>
#include <stdlib.h>





unsigned at_lib_table_make(atVM *vm, unsigned arg_count);
unsigned at_lib_table_copy(atVM *vm, unsigned arg_count);

/**
 * Loads the IO library onto the VM
 * @param vm The VM
 */
void at_lib_load_table(atVM *vm) {

	// create a table for the table library
	atTable table_table;
	at_table_init(vm, &table_table);

	// add functions
	at_table_set_cfunction(&table_table, "make", at_lib_table_make);
	//at_table_set_cfunction(&table_table, "copy", at_lib_table_make);

	// store the table table
	at_vm_push_table(vm, &table_table);
	at_vm_store(vm, "table");
}

unsigned at_lib_table_make(atVM *vm, unsigned arg_count) {
	atTable table;
	at_table_init(vm, &table);
	at_vm_push_table(vm, &table);

	return 1;
}
