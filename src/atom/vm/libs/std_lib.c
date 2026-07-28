/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/26/2018
 * @Filename: std_lib.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/06/2018
 */

#include "atom/vm/libs/std_lib.h"
#include "atom/vm/vm.h"
#include "atom/util/file_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned at_lib_std_exit(atVM *vm, unsigned arg_count);
unsigned at_lib_std_typeof(atVM *vm, unsigned arg_count);
unsigned at_lib_std_to_number(atVM *vm, unsigned arg_count);
unsigned at_lib_std_to_string(atVM *vm, unsigned arg_count);
unsigned at_lib_std_system(atVM *vm, unsigned arg_count);

unsigned at_lib_std_eval(atVM *vm, unsigned arg_count);
unsigned at_lib_std_require(atVM *vm, unsigned arg_count);

/**
* Loads the standard library onto the VM
* @param vm The VM
*/
void at_lib_load_std(atVM *vm) {

	// load values
	at_vm_push_number(vm, ATOM_TRUE);
	at_vm_store(vm, "true");
	at_vm_push_number(vm, ATOM_FALSE);
	at_vm_store(vm, "false");

	// load functions
	at_vm_push_cfunction(vm, at_lib_std_exit);
	at_vm_store(vm, "exit");

	at_vm_push_cfunction(vm, at_lib_std_typeof);
	at_vm_store(vm, "typeof");

	at_vm_push_cfunction(vm, at_lib_std_to_number);
	at_vm_store(vm, "to_number");

	at_vm_push_cfunction(vm, at_lib_std_to_string);
	at_vm_store(vm, "to_string");

	at_vm_push_cfunction(vm, at_lib_std_system);
	at_vm_store(vm, "system");

	at_vm_push_cfunction(vm, at_lib_std_require);
	at_vm_store(vm, "import");

	atTable std_table;
	at_table_init(vm, &std_table);

	at_table_set_cfunction(&std_table, "eval", at_lib_std_eval);

	at_vm_push_table(vm, &std_table);
	at_vm_store(vm, "std");

}

unsigned at_lib_std_exit(atVM *vm, unsigned arg_count) {
	exit(0);
	return 0;
}

unsigned at_lib_std_typeof(atVM *vm, unsigned arg_count) {

	// get the value to determine the type of
	atValue value = *at_vm_top(vm);
	at_value_reference(&value);
	at_vm_pop(vm);

	// check if the value is null
	if ( at_value_is_null(&value) ) {
		at_value_dereference(&value);
		at_vm_push_string(vm, "null");
		return 1;
	}

	// return the value type as a string
	switch ( value.type ) {
		case ATOM_VALUE_NUMBER:
			at_vm_push_string(vm, "number");
			break;

		case ATOM_VALUE_STRING:
			at_vm_push_string(vm, "string");
			break;

		case ATOM_VALUE_FUNCTION:
			at_vm_push_string(vm, "function");
			break;

		case ATOM_VALUE_USER_FUNCTION:
			at_vm_push_string(vm, "userfunction");
			break;

		case ATOM_VALUE_TABLE:
			at_vm_push_string(vm, "table");
			break;

		default:
			at_vm_push_string(vm, "unknown");
			break;
	}

	at_value_dereference(&value);
	return 1;
}

unsigned at_lib_std_to_number(atVM *vm, unsigned arg_count) {

	// get the value to convert
	atValue value = *at_vm_top(vm);
	at_value_reference(&value);
	at_vm_pop(vm);


	// check if the value is invalid
	if ( at_value_is_null(&value) ) {
		at_vm_push_number(vm, 0);
	}
	// convert a string to a number
	else if ( value.type == ATOM_VALUE_STRING ) {
		at_vm_push_number(vm, atof((char*)value.raw_value));
	}
	// convert a number to a number
	else if ( value.type == ATOM_VALUE_NUMBER ) {
		at_vm_push(vm, &value);
	}
	// default
	else {
		at_vm_push_number(vm, 0);
	}

	at_value_dereference(&value);

	return 1;
}
unsigned at_lib_std_to_string(atVM *vm, unsigned arg_count) {
	// get the value to convert
	atValue *arg_value = at_vm_top(vm);
	at_value_reference(arg_value);



	// check if the value is invalid
	if ( at_value_is_null(arg_value) ) {
		at_vm_push_string(vm, "null");
		at_vm_pop(vm);
		return 1;
	}

	atValue value = *arg_value;
	at_vm_pop(vm);



	// convert a number to a string
	if ( value.type == ATOM_VALUE_NUMBER ) {

		atom_number num = at_value_to_number(&value);

		// get rid of the negative sign that sometimes shows up (e.g. -0.0000)
		if ( num == -0.0 )
			num = 0.0;

		char str[64];

		// print the number as either an int or a float
		if ( num - (double)(int)num == 0.0 )
			sprintf(str, "%d", (int)num);
		else
			sprintf(str, "%f", num);

		at_vm_push_string(vm, str);
	}
	// convert a string to a string
	else if ( value.type == ATOM_VALUE_STRING ) {
		at_vm_push(vm, &value);
	}
	// default
	else {
		at_vm_push_string(vm, "null");
	}

	at_value_dereference(&value);

	return 1;
}


unsigned at_lib_std_system(atVM *vm, unsigned arg_count) {
	// get the command to run
	atValue *value = at_vm_top(vm);
	at_value_reference(value);
	at_vm_pop(vm);

	if ( at_value_is_string(value) ) {
		system((char*)value->raw_value);
	}

	return 0;
}

unsigned at_lib_std_require(atVM *vm, unsigned arg_count) {
	// get the command to run
	atValue *value = at_vm_top(vm);
	at_value_reference(value);
	at_vm_pop(vm);

	if ( at_value_is_string(value) ) {
		const char *file = (char*)value->raw_value;
		char *source;

		// read the source file
		if ( !at_read_text_file(file, &source) ) {
			printf("Failed to import file '%s'\n", file);
			return 0;
		}

		printf("%s\n", source);

		// execute the source
		at_executor_run_source(&vm->executor, source);

		// free the source string
		free(source);
	}
	else {
		printf("Error: cannot import non-string value\n");
	}

	return 0;
}


unsigned at_lib_std_eval(atVM *vm, unsigned arg_count) {

	// get the string
	atValue *string_value = at_vm_top(vm);


	atCallFrame *top_frame = at_executor_top_frame(&vm->executor);


	// check if the argument is null
	if ( at_value_is_null(string_value) ) {
		at_vm_pop(vm);
		printf("Cannot evaluate null value\n");
		at_vm_push_null(vm);

		return 1;
	}

	atValue string_copy = *string_value;
	at_value_reference(&string_copy);
	at_vm_pop(vm);


	unsigned op_size = at_frame_size(top_frame);
	char *string = (char*)string_copy.raw_value;


	at_executor_run_source(&vm->executor, string);

	at_value_dereference(&string_copy);

	int new_op_size = at_frame_size(top_frame);

	if ( op_size == new_op_size || new_op_size == 0 ) {
		at_vm_push_null(vm);
	}
	else {
		return new_op_size - op_size;
	}

	return 1;
}
