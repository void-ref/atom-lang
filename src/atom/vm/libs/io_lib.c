/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/22/2018
 * @Filename: io_lib.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#include "atom/vm/libs/io_lib.h"
#include "atom/vm/vm.h"
#include "atom/util/memory_util.h"

#include <stdio.h>
#include <stdlib.h>





unsigned at_lib_io_print(atVM *vm, unsigned arg_count);
unsigned at_lib_io_getline(atVM *vm, unsigned arg_count);
unsigned at_lib_io_readfile(atVM *vm, unsigned arg_count);
unsigned at_lib_io_writefile(atVM *vm, unsigned arg_count);

/**
 * Loads the IO library onto the VM
 * @param vm The VM
 */
void at_lib_load_io(atVM *vm) {

	// create a table for the IO library
	atTable io_table;
	at_table_init(vm, &io_table);

	// add functions
	at_table_set_cfunction(&io_table, "print", at_lib_io_print);
	at_table_set_cfunction(&io_table, "getline",  at_lib_io_getline);
	at_table_set_cfunction(&io_table, "readfile",  at_lib_io_readfile);
	at_table_set_cfunction(&io_table, "writefile",  at_lib_io_writefile);

	// store the IO table
	at_vm_push_table(vm, &io_table);
	at_vm_store(vm, "io");
}



unsigned at_lib_io_print(atVM *vm, unsigned arg_count) {

	// get the value to print
	atValue *value = at_vm_top(vm);

	// check if the value is null
	if ( value == NULL || value->type == ATOM_VALUE_NULL ) {
		printf("null\n");
		return 0;
	}

	// print the value depending on its type
	switch ( value->type ) {
		case ATOM_VALUE_STRING:
			printf("%s\n", (char*)value->raw_value);
			break;

		case ATOM_VALUE_NUMBER: {
			atom_number num = *(atom_number*)value->raw_value;

			// get rid of the negative sign that sometimes shows up (e.g. -0.0000)
			if ( num == 0 ) num = 0;

			// print the number
			if ( (int)num == num )
				printf("%d\n", (int)num);
			else
				printf("%f\n", num);

			break;
		}


		case ATOM_VALUE_FUNCTION:
			printf("function %p\n", value->raw_value);
			break;

		case ATOM_VALUE_USER_FUNCTION:
			printf("user function %p\n", value->raw_value);
			break;

		case ATOM_VALUE_TABLE:
			printf("table %p\n", value->raw_value);
			break;

		default:
			printf("invalid type\n");
			break;
	}

	// pop the value from the VM
	at_vm_pop(vm);

	return 0;
}



unsigned at_lib_io_getline(atVM *vm, unsigned arg_count) {

	unsigned buff_capacity = 128;
	char *buff = malloc(buff_capacity);
	int c = 0;
	int i = 0;

	// read characters until a newline is reached
	while ( (c = getchar()) != EOF && c != '\n' ) {

		// check if the buffer needs to be resized
		if ( i + 1 >= buff_capacity ) {
			buff_capacity *= 2;
			char *new_buff = at_array_copy(buff, i, buff_capacity);
			free(buff);
			buff = new_buff;
		}

		// insert the next character
		buff[i++] = c;
	}

	// inser the null-terminator character
	buff[i++] = '\0';

	// push the string onto the VM
	at_vm_push_string(vm, buff);

	// free the buffer
	free(buff);


	return 1;
}

unsigned at_lib_io_readfile(atVM *vm, unsigned arg_count) {

	// get the file name
	atValue *file_name_value = at_vm_top(vm);
	at_value_reference(file_name_value);
	at_vm_pop(vm);

	// if the file name is invalid, push null
	if ( !at_value_is_string(file_name_value) ) {
		at_vm_push_null(vm);
		at_value_dereference(file_name_value);
		return 1;
	}


	char *file_name = (char*)file_name_value->raw_value;

	// try to open the file
	FILE *file = fopen(file_name, "r");
	if ( !file ) {
		at_vm_push_null(vm);
		at_value_dereference(file_name_value);
		return 1;
	}
	else {
		at_value_dereference(file_name_value);
	}

	// get the length of the file
	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// read the file into a buffer
	char *buff = malloc(length);
	fread(buff, 1, length, file);

	// close the file
	fclose(file);

	// push the buffer onto the vm
	at_vm_push_string(vm, buff);

	// free the buffer
	free(buff);



	return 1;
}
unsigned at_lib_io_writefile(atVM *vm, unsigned arg_count) {
	// get the file name
	atValue file_name_value = *at_vm_top(vm);
	at_value_reference(&file_name_value);
	at_vm_pop(vm);

	// get the file contents
	atValue file_contents_value = *at_vm_top(vm);
	at_value_reference(&file_contents_value);
	at_vm_pop(vm);

	// if the file name or contents are invalid, push false
	if ( !at_value_is_string(&file_name_value) || !at_value_is_string(&file_contents_value) ) {
		printf("Invalid argument to function.\n");
		at_vm_push_number(vm, ATOM_FALSE);
		at_value_dereference(&file_name_value);
		at_value_dereference(&file_contents_value);
		return 1;
	}

	char *file_name = (char*)file_name_value.raw_value;
	char *file_contents = (char*)file_contents_value.raw_value;

	// try to open the file
	FILE *file = fopen(file_name, "w");
	if ( !file ) {
		printf("Could not open file '%s'\n", file_name);
		at_vm_push_number(vm, ATOM_FALSE);
		at_value_dereference(&file_name_value);
		at_value_dereference(&file_contents_value);
		return 1;
	}



	// write the contents to the file
	fprintf(file, "%s", file_contents);

	// close the file
	fclose(file);

	at_value_dereference(&file_name_value);
	at_value_dereference(&file_contents_value);

	// push true to indicate success
	at_vm_push_number(vm, ATOM_TRUE);



	return 1;
}
