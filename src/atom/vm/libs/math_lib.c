/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/22/2018
 * @Filename: math_lib.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#include "atom/vm/libs/math_lib.h"
#include "atom/vm/vm.h"
#include <math.h>



// trig functions
unsigned at_lib_math_sin(atVM *vm, unsigned arg_count);
unsigned at_lib_math_cos(atVM *vm, unsigned arg_count);
unsigned at_lib_math_tan(atVM *vm, unsigned arg_count);
unsigned at_lib_math_asin(atVM *vm, unsigned arg_count);
unsigned at_lib_math_acos(atVM *vm, unsigned arg_count);
unsigned at_lib_math_atan(atVM *vm, unsigned arg_count);

// exponent/log functions
unsigned at_lib_math_sqrt(atVM *vm, unsigned arg_count);
unsigned at_lib_math_cbrt(atVM *vm, unsigned arg_count);
unsigned at_lib_math_log(atVM *vm, unsigned arg_count);
unsigned at_lib_math_log10(atVM *vm, unsigned arg_count);
unsigned at_lib_math_logbase(atVM *vm, unsigned arg_count);

// misc functions
unsigned at_lib_math_abs(atVM *vm, unsigned arg_count);
unsigned at_lib_math_sign(atVM *vm, unsigned arg_count);
unsigned at_lib_math_floor(atVM *vm, unsigned arg_count);
unsigned at_lib_math_ceil(atVM *vm, unsigned arg_count);



/**
 * Loads the math library onto the VM
 * @param vm The VM
 */
void at_lib_load_math(atVM *vm) {

	// create a table for the math library
	atTable math_table;
	at_table_init(vm, &math_table);

	// add some constants
	at_table_set_number(&math_table, "pi", M_PI);
	at_table_set_number(&math_table, "e", M_E);

	// add trig functions
	at_table_set_cfunction(&math_table, "sin",   at_lib_math_sin);
	at_table_set_cfunction(&math_table, "cos",   at_lib_math_cos);
	at_table_set_cfunction(&math_table, "tan",   at_lib_math_cos);
	at_table_set_cfunction(&math_table, "asin",  at_lib_math_asin);
	at_table_set_cfunction(&math_table, "acos",  at_lib_math_acos);
	at_table_set_cfunction(&math_table, "atan",  at_lib_math_atan);

	// add exponent/log functions
	at_table_set_cfunction(&math_table, "sqrt",  at_lib_math_sqrt);
	at_table_set_cfunction(&math_table, "cbrt",  at_lib_math_cbrt);
	at_table_set_cfunction(&math_table, "log",   at_lib_math_log);
	at_table_set_cfunction(&math_table, "log10", at_lib_math_log10);
	at_table_set_cfunction(&math_table, "logb",  at_lib_math_logbase);

	// add misc functions
	at_table_set_cfunction(&math_table, "abs",   at_lib_math_abs);
	at_table_set_cfunction(&math_table, "sign",  at_lib_math_sign);
	at_table_set_cfunction(&math_table, "floor", at_lib_math_floor);
	at_table_set_cfunction(&math_table, "ceil",  at_lib_math_ceil);

	// Store the table in the VM
	at_vm_push_table(vm, &math_table);
	at_vm_store(vm, "math");
}



unsigned at_lib_math_sin(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, sin(arg));

	return 1;
}
unsigned at_lib_math_cos(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, cos(arg));

	return 1;
}
unsigned at_lib_math_tan(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, tan(arg));

	return 1;
}
unsigned at_lib_math_asin(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, asin(arg));

	return 1;
}
unsigned at_lib_math_acos(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, acos(arg));

	return 1;
}
unsigned at_lib_math_atan(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, atan(arg));

	return 1;
}


unsigned at_lib_math_sqrt(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, sqrt(arg));

	return 1;
}
unsigned at_lib_math_cbrt(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, cbrt(arg));

	return 1;
}

unsigned at_lib_math_log(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, log(arg));

	return 1;
}
unsigned at_lib_math_log10(atVM *vm, unsigned arg_count) {
	atom_number arg;
	at_vm_pop_number(vm, &arg);
	at_vm_push_number(vm, log10(arg));

	return 1;
}
unsigned at_lib_math_logbase(atVM *vm, unsigned arg_count) {
	atom_number num, base;
	at_vm_pop_number(vm, &num);
	at_vm_pop_number(vm, &base);
	at_vm_push_number(vm, log(num) / log(base));

	return 1;
}


unsigned at_lib_math_abs(atVM *vm, unsigned arg_count) {
	atom_number num;
	at_vm_pop_number(vm, &num);
	at_vm_push_number(vm, num < 0 ? -num : num);

	return 1;
}

unsigned at_lib_math_sign(atVM *vm, unsigned arg_count) {
	atom_number num;
	at_vm_pop_number(vm, &num);
	at_vm_push_number(vm, num <= 0 ? (num == 0 ? 0 : -1) : 1);

	return 1;
}

unsigned at_lib_math_floor(atVM *vm, unsigned arg_count) {
	atom_number num;
	at_vm_pop_number(vm, &num);
	at_vm_push_number(vm, floor(num));

	return 1;
}

unsigned at_lib_math_ceil(atVM *vm, unsigned arg_count) {
	atom_number num;
	at_vm_pop_number(vm, &num);
	at_vm_push_number(vm, ceil(num));

	return 1;
}
