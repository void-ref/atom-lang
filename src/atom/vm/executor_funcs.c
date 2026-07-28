/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/23/2018
 * @Filename: executor_funcs.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/06/2018
 */

#include "atom/vm/executor_funcs.h"
#include "atom/vm/executor.h"
#include "atom/vm/frame.h"
#include "atom/vm/table.h"
#include "atom/vm/bytecode.h"
#include "atom/vm/function.h"
#include "atom/vm/vm.h"
#include "atom/parser/parser.h"
#include "atom/util/memory_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>


// Holds function pointers for executor functions. The function pointers are stored
// in an index corresponding to their bytecode instruction number
atExecutorFunction _at_executor_functions[ATOM_BC_COUNT] = {0};


/**
 * Initializes the global executor function array.
 */
void _at_executor_init_functions() {
	// avoid re-initialization of the array
	if ( _at_executor_functions[0] != 0 )
		return;

	_at_executor_functions[ATOM_BC_HALT] = _at_executor_do_halt;
	//_at_executor_functions[ATOM_BC_TERMINATE] = _at_executor_do_terminate;
	_at_executor_functions[ATOM_BC_JUMP] = _at_executor_do_jump;
	_at_executor_functions[ATOM_BC_CJUMP] = _at_executor_do_cjump;

	// push primitives
	_at_executor_functions[ATOM_BC_PUSH_NULL] = _at_executor_do_push_null;
	_at_executor_functions[ATOM_BC_PUSH_NUMBER] = _at_executor_do_push_number;
	_at_executor_functions[ATOM_BC_PUSH_STRING] = _at_executor_do_push_string;

	// load / store
	_at_executor_functions[ATOM_BC_LOAD_VALUE] = _at_executor_do_load_value;
	_at_executor_functions[ATOM_BC_STORE] = _at_executor_do_store;
	_at_executor_functions[ATOM_BC_STORE_LOCAL] = _at_executor_do_store_local;
	_at_executor_functions[ATOM_BC_STORE_TABLE] = _at_executor_do_store_table;

	// function
	_at_executor_functions[ATOM_BC_NEW_FUNCTION] = _at_executor_do_new_function;
	_at_executor_functions[ATOM_BC_LOAD_ARGUMENT] = _at_executor_do_load_argument;
	_at_executor_functions[ATOM_BC_CALL] = _at_executor_do_call;
	//_at_executor_functions[ATOM_BC_RETURN] = _at_executor_do_return;

	// table
	//_at_executor_functions[ATOM_BC_NEW_TABLE] = _at_executor_do_new_table;
	_at_executor_functions[ATOM_BC_INDEX] = _at_executor_do_index;



	// arithmetic
	_at_executor_functions[ATOM_BC_ADD]    = _at_executor_do_add;
	_at_executor_functions[ATOM_BC_SUB]    = _at_executor_do_sub;
	_at_executor_functions[ATOM_BC_MUL]    = _at_executor_do_mul;
	_at_executor_functions[ATOM_BC_DIV]    = _at_executor_do_div;
	_at_executor_functions[ATOM_BC_MOD]    = _at_executor_do_mod;
	_at_executor_functions[ATOM_BC_EXP]    = _at_executor_do_exp;
	_at_executor_functions[ATOM_BC_NEGATE] = _at_executor_do_neg;
	_at_executor_functions[ATOM_BC_FACT]   = _at_executor_do_fact;

	// value comparison
	_at_executor_functions[ATOM_BC_COMP_EQ] = _at_executor_do_eq;
	_at_executor_functions[ATOM_BC_COMP_NEQ] = _at_executor_do_neq;
	_at_executor_functions[ATOM_BC_COMP_GT] = _at_executor_do_gt;
	_at_executor_functions[ATOM_BC_COMP_GTE] = _at_executor_do_gte;
	_at_executor_functions[ATOM_BC_COMP_LT] = _at_executor_do_lt;
	_at_executor_functions[ATOM_BC_COMP_LTE] = _at_executor_do_lte;

	// boolean comparison
	_at_executor_functions[ATOM_BC_NOT] = _at_executor_do_not;
	_at_executor_functions[ATOM_BC_AND] = _at_executor_do_and;
	_at_executor_functions[ATOM_BC_OR] = _at_executor_do_or;
	_at_executor_functions[ATOM_BC_XOR] = _at_executor_do_xor;
}


atom_bool _at_executor_try_pop(atVM *vm, atValue *out, atom_bool allow_null);

/**
 * Executes the next instruction in the given bytecode array
 * @param  vm    The VM to run the bytecode on
 * @param  start A pointer to the starting instruction
 * @param  end   A pointer to the end of the bytecode array
 * @return       A pointer to the new position in the bytecode array
 */
atom_byte* _at_executor_do_next_instruction(atVM *vm, atom_byte *start, atom_byte *end) {


	// initialize the executor function array if it is not already
	if ( _at_executor_functions[0] == 0 )
		_at_executor_init_functions();



	// get the next instruction
	atom_byte instruction = *start++;

	// printf("Executing instruction %s\n", at_bc_get_name(instruction));

	// make sure the instruction is valid
	if ( instruction >= ATOM_BC_COUNT ) {
		printf("Error: malformed bytecode; could not execute instruction %d.\n", instruction);
		return end;
	}
	else if ( _at_executor_functions[instruction] == 0 ) {
		printf("Error: unimplemented instruction '%s'.\n", at_bc_get_name(instruction));
		return end;
	}

	// execute the instruction
	return _at_executor_functions[instruction](vm, start, end);
}

/**
 * Halts execution
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_halt(atVM *vm, atom_byte *p, atom_byte *end) {
	return end;
}

/**
 * Performs the push_null operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_push_null(atVM *vm, atom_byte *p, atom_byte *end) {
	at_vm_push_null(vm);

	return p;
}

/**
 * Performs the push_number operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_push_number(atVM *vm, atom_byte *p, atom_byte *end) {

	// read the number from the bytecode
	atom_number num;
	p = at_bc_next_number(p, 0, end - p, &num);

	// check if the number was successfully read
	if ( p == NULL ) {
		printf("Error: malformed bytecode; could not read number.\n");
		return end;
	}
	else {
		// push the number onto the VM
		at_vm_push_number(vm, num);
		return p;
	}
}

/**
 * Performs the push_string operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_push_string(atVM *vm, atom_byte *p, atom_byte *end) {

	// Read the string from the bytecode
	char *s;
	p = at_bc_next_string(p, 0, end - p, &s);

	// check if the string was successfully read
	if ( p == NULL ) {
		printf("Error: malformed bytecode; could not read string.\n");
		return end;
	}
	else {
		// push the number onto the VM
		at_vm_push_string(vm, s);
		return p;
	}
}


/**
 * Performs the load_value operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_load_value(atVM *vm, atom_byte *p, atom_byte *end) {

	// read the name of the value from the bytecode
	char *name;
	p = at_bc_next_string(p, 0, end - p, &name);

	// check if the string was successfully read
	if ( p == NULL ) {
		printf("Error: malformed bytecode; could not read storage locator.\n");
		return end;
	}


	// store some useful values
	atExecutor *exe = &vm->executor;
	atCallFrame *top_frame = at_executor_top_frame(exe);
	atCallFrame *frame;
	atValue *value;

	// search up through the call frames, starting from the current frame
	for (int i = exe->call_stack_size - 1; i >= 0; --i) {

		frame = exe->call_stack + i;

		// check if the value was found
		at_frame_load(frame, name);
		value = at_frame_top(frame);

		// pop the value if it is null
		if ( value == NULL ) {
			at_frame_pop(frame);
		}
		// if it is not null, then success!
		else {

			// if the index is not the top index, we have to copy the value
			// into the top frame
			if ( i != exe->call_stack_size - 1 ) {
				at_frame_push(top_frame, value);
				at_frame_pop(frame);
			}

			break;
		}
	}

	return p;
}

/**
 * Performs the store operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_store(atVM *vm, atom_byte *p, atom_byte *end) {

	// get the global call frame (index zero on the call stack)
	atCallFrame *global = &vm->executor.call_stack[0];

	// get the key to store the value in
	char *name;
	p = at_bc_next_string(p, 0, end - p, &name);

	// check if the string was successfully read
	if ( p == NULL ) {
		printf("Error: malformed bytecode; could not read storage locator.\n");
		return end;
	}

	// store the value
	at_frame_store(global, name);

	return p;

}

/**
 * Performs the table store operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_store_table(atVM *vm, atom_byte *p, atom_byte *end) {

	atValue *table_value;
	atValue *key_value;
	atValue *value_value;
	atValue value, key, table;
	atom_bool arg_success = ATOM_TRUE;



	// pop the value
	value_value = at_vm_top(vm);
	if ( value_value == NULL ) {
		printf("Setting null\n");
		at_value_set_null(&value);
	}
	else {
		value = *value_value;
		at_value_reference(&value);
	}
	at_vm_pop(vm);

	// pop the key
	key_value = at_vm_top(vm);
	if ( at_value_is_null(key_value) || key_value->type != ATOM_VALUE_STRING )  {
		p = end;
		arg_success = ATOM_FALSE;
		printf("Error: cannot index table with invalid key.\n");
	}
	else {
		key = *key_value;
		at_value_reference(&key);
	}
	at_vm_pop(vm);

	// pop the table
	table_value = at_vm_top(vm);
	if ( at_value_is_null(table_value) || table_value->type != ATOM_VALUE_TABLE )  {
		p = end;
		arg_success = ATOM_FALSE;
		printf("Error: cannot index non-table value.\n");
	}
	else {
		table = *table_value;
		at_value_reference(&table);
	}
	at_vm_pop(vm);


	// Execute if the arguments are valid
	if ( arg_success ) {
		at_table_set((atTable*)table.raw_value, (char*)key.raw_value, &value);
	}
	else {
		printf("Could not index table.\n");
	}

	at_value_dereference(&value);
	at_value_dereference(&key);
	at_value_dereference(&table);

	return p;
}

/**
 * Performs the store_local operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_store_local(atVM *vm, atom_byte *p, atom_byte *end) {

	// get the key to store the value in
	char *name;
	p = at_bc_next_string(p, 0, end - p, &name);

	// store the value in the top frame
	atCallFrame *frame = at_executor_top_frame(&vm->executor);
	at_frame_store(frame, name);

	return p;
}


/**
 * Performs the index operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_index(atVM *vm, atom_byte *p, atom_byte *end) {


	atCallFrame *frame = at_executor_top_frame(&vm->executor);



	// get the key value
	atValue *key_value = at_frame_top(frame);
	at_value_reference(key_value);
	at_frame_pop(frame);

	// get the table value
	atValue *table_value = at_frame_top(frame);
	at_value_reference(table_value);
	at_frame_pop(frame);

	atValue value;


	// load the value from the table
	if ( table_value == NULL || table_value->type == ATOM_VALUE_NULL ) {
		printf("Error: cannot index null value.\n");
		p = end;
		at_value_set_null(&value);
	}
	else if ( table_value->type != ATOM_VALUE_TABLE ) {
		printf("Error: cannot index non-table value.\n");
		p = end;
		at_value_set_null(&value);
	}
	else if ( key_value == NULL || key_value->type == ATOM_VALUE_NULL ) {
		printf("Error: cannot index table with null key.\n");
		p = end;
		at_value_set_null(&value);
	}
	else {
		char *key = (char*)key_value->raw_value;

		atTable *table = (atTable*)table_value->raw_value;
		atValue *value_ptr = at_table_get(table, key);
		if ( at_value_is_null(value_ptr) )
			at_value_set_null(&value);
		else
			value = *value_ptr;
	}


	at_value_dereference(table_value);
	at_value_dereference(key_value);

	at_frame_push(frame, &value);

	return p;
}


/**
 * Performs the new_function operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_new_function(atVM *vm, atom_byte *p, atom_byte *end) {

	// read the function length
	atom_int func_length = *(atom_int*)p;
	p += sizeof(atom_int);

	// store the starting pointer of the function
	atom_byte *start = p;

	// move the pointer past the function
	p += func_length;

	// initialize a new function
	atFunction func;
	at_function_init(&func, start, func_length);

	// create a new value to hold the function
	atValue new_value;
	at_value_from_function(&new_value, &func);

	// push the function onto the current call frame
	at_vm_push(vm, &new_value);

	return p;
}

/**
 * Performs the call operation
 * @param  vm    The VM
 * @param  p     The start pointer
 * @param  end   The end pointer
 * @return       The new position
 */
atom_byte* _at_executor_do_call(atVM *vm, atom_byte *p, atom_byte *end) {


	// store some useful values
	atExecutor *exe = &vm->executor;
	atCallFrame *current_frame = at_executor_top_frame(exe);

	// get the number of arguments to the function
	atom_byte arg_count;
	p = at_bc_next_byte(p, 0, end - p, &arg_count);

	// push a new frame for the function call
	atCallFrame *new_frame = at_executor_push_frame(exe);



	// load the arguments onto the new frame in reverse order
	for (int i = arg_count - 1; i >=0; --i) {

		// get the next argument
		atValue *arg = at_frame_top(current_frame);

		// push the argument onto the new frame
		at_frame_push(new_frame, arg);

		// pop the value from the parent frame
		at_frame_pop(current_frame);
	}

	// get the function to call
	atValue *func_value = at_frame_top(current_frame);
	at_value_reference(func_value);
	at_frame_pop(current_frame);

	// check if the function is valid
	if ( at_value_is_null(func_value) ) {
		printf("Error: cannot call a null value\n");
		return end;
	}
	// call a user function
	else if ( func_value->type == ATOM_VALUE_USER_FUNCTION ) {

		// get a user function from the value
		atUserFunction user_func = (atUserFunction)func_value->raw_value;

		// call the user function. Its return value is how many
		// values were pushed onto the stack
		unsigned pushed = user_func(exe->vm, arg_count);

		// copy the return value(s) from the new call frame to the old one
		for (int i = 0; i < pushed; ++i) {
			atValue *return_value = at_frame_top(new_frame);
			at_frame_push(current_frame, return_value);
			at_frame_pop(new_frame);
		}

		// pop the call frame
		at_executor_pop_frame(exe);

		return p;
	}
	else if ( func_value->type == ATOM_VALUE_FUNCTION ) {

		// store the function arguments
		for (int i = 0; i < arg_count; ++i) {

			// use a key that is syntactically invalid to
			// hold the argument. This will ensure it cannot
			// be accidentally indexed by ordinary identifiers
			char key[3] = ".";
			key[1] = 'A' + i;
			key[2] = '\0';

			// store the argument
			at_frame_store(new_frame, key);
		}


		atFunction *func = (atFunction*)func_value->raw_value;

		// call the executor with the bytecode belonging to the function
		at_executor_run_bytecode(exe, func->bytecode, func->bytecode_length);

		// pop the call frame
		at_executor_pop_frame(exe);
	}
	// here the value must be a non-function value
	else if ( func_value->type != ATOM_VALUE_FUNCTION ) {
		printf("Error: cannot call non-function value\n");
		return end;
	}

	return p;
}



/**
 * Performs the argument load operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_load_argument(atVM *vm, atom_byte *p, atom_byte *end) {

	// read the argument index
	atom_byte arg_index;
	p = at_bc_next_byte(p, 0, end - p, &arg_index);

	// get the argument key
	char key[3] = ".";
	key[1] = 'A' + arg_index;
	key[2] = '\0';

	// load the argument
	atCallFrame *frame = at_executor_top_frame(&vm->executor);
	at_frame_load(frame, key);

	return p;
}

atom_bool _at_executor_try_pop(atVM *vm, atValue *out, atom_bool allow_null) {
	atom_bool success = ATOM_TRUE;

	// get the operand
	atValue *top = at_vm_top(vm);


	// check if the operand is valid
	if ( at_value_is_null(top) ) {
		at_value_set_null(out);
		if ( !allow_null )
			success = ATOM_FALSE;
	}
	// set the output
	else {
		at_value_reference(top);
		*out = *top;
	}

	// pop the operand
	at_vm_pop(vm);

	return success;
}

atom_byte* _at_executor_do_add(atVM *vm, atom_byte *p, atom_byte *end) {
	atValue left_value, right_value;
	atValueType left_type, right_type;

	atom_bool success = ATOM_TRUE;
	success = success && _at_executor_try_pop(vm, &right_value, ATOM_FALSE);
	success = success && _at_executor_try_pop(vm, &left_value, ATOM_FALSE);

	// check if the operands are non-null
	if ( !success ) {
		printf("Error: null operand in addition.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		left_type = left_value.type;
		right_type = right_value.type;


		// regular addition
		if ( left_type == ATOM_VALUE_NUMBER && right_type == ATOM_VALUE_NUMBER ) {
			at_vm_push_number(vm, at_value_to_number(&left_value) + at_value_to_number(&right_value));
		}
		// string concatenation
		else if ( left_type == ATOM_VALUE_STRING && right_type == ATOM_VALUE_STRING ) {
			char *left_str  = (char*)left_value.raw_value;
			char *right_str = (char*)right_value.raw_value;
			unsigned left_len = strlen(left_str);
			unsigned right_len = strlen(right_str);

			// copy each string into the new string
			char *total = malloc(left_len + right_len + 1);
			memcpy(total, left_str, left_len);
			memcpy(total + left_len, right_str, right_len);
			total[left_len + right_len] = '\0';

			// push the new string onto the stack
			atValue new_str;
			at_value_from_string_nocopy(&new_str, total);
			at_vm_push(vm, &new_str);
		}
		else {
			printf("Error: invalid operand in addition.\n");
			at_vm_push_null(vm);
			p = end;
		}

	}

	// dereference the operands
	at_value_dereference(&left_value);
	at_value_dereference(&right_value);

	return p;
}
atom_byte* _at_executor_do_sub(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number left, right;
	atom_bool success;

	// get the two operands
	success = at_vm_pop_number(vm, &right);
	success = at_vm_pop_number(vm, &left) && success;

	// check if the operands were popped successfully
	if ( !success ) {
		printf("Error: invalid operand in subtraction.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, left - right);
	}

	return p;
}

atom_byte* _at_executor_do_mul(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number left, right;
	atom_bool success;

	// get the two operands
	success = at_vm_pop_number(vm, &right);
	success = at_vm_pop_number(vm, &left) && success;

	// check if the operands were popped successfully
	if ( !success ) {
		printf("Error: invalid operand in multiplication.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, left * right);
	}

	return p;
}

atom_byte* _at_executor_do_div(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number left, right;
	atom_bool success;

	// get the two operands
	success = at_vm_pop_number(vm, &right);
	success = at_vm_pop_number(vm, &left) && success;

	// check if the operands were popped successfully
	if ( !success ) {
		printf("Error: invalid operand in division.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, right == 0 ? 0 : left / right);
	}

	return p;
}

atom_byte* _at_executor_do_mod(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number left, right;
	atom_bool success;

	// get the two operands
	success = at_vm_pop_number(vm, &right);
	success = at_vm_pop_number(vm, &left) && success;

	// check if the operands were popped successfully
	if ( !success ) {
		printf("Error: invalid operand in modulo.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, right == 0 ? 0 : fmod(left, right));
	}

	return p;
}

atom_byte* _at_executor_do_exp(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number left, right;
	atom_bool success;

	// get the two operands
	success = at_vm_pop_number(vm, &right);
	success = at_vm_pop_number(vm, &left) && success;

	// check if the operands were popped successfully
	if ( !success ) {
		printf("Error: invalid operand in power.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, pow(left, right));
	}

	return p;
}

atom_byte* _at_executor_do_neg(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number num;

	// check if the operand was popped successfully
	if ( !at_vm_pop_number(vm, &num) ) {
		printf("Error: invalid operand in negation.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, -num);
	}

	return p;
}

atom_byte* _at_executor_do_fact(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number num;

	// check if the operand was popped successfully
	if ( !at_vm_pop_number(vm, &num) ) {
		printf("Error: invalid operand in factorial.\n");
		at_vm_push_null(vm);
		p = end;
	}
	else {
		at_vm_push_number(vm, tgamma(num + 1));
	}

	return p;
}




/**
 * Performs the jump operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_jump(atVM *vm, atom_byte *p, atom_byte *end) {

	atom_int jump_offset = *(atom_int*)p;
	p += sizeof(atom_int);

	p = p + jump_offset;

	//printf("jumping\n");
	return p;

}

/**
 * Performs the conditional jump operation
 * @param  vm  The VM
 * @param  p   The bytecode start pointer
 * @param  end The end pointer
 * @return     The new position
 */
atom_byte* _at_executor_do_cjump(atVM *vm, atom_byte *p, atom_byte *end) {
	atom_number condition;

	// check if the condition is valid
	if ( !at_vm_pop_number(vm, &condition) ) {
		printf("Error: non-boolean value as condition");
		p = end;
	}
	else {


		atom_int jump_offset = *(atom_int*)p;
		p += sizeof(atom_int);

		// handle the false condition
		if ( condition == 0 ) {
			//printf("cjumping\n");
			p = p + jump_offset;
		}
		else {
			//printf("not cjumping\n");
		}



	}


	return p;
}



atom_byte* _at_executor_do_eq(atVM *vm, atom_byte *p, atom_byte *end) {

	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_equals(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_neq(atVM *vm, atom_byte *p, atom_byte *end) {

	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_nequals(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_gt(atVM *vm, atom_byte *p, atom_byte *end) {

	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_gt(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_gte(atVM *vm, atom_byte *p, atom_byte *end) {

	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_gte(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_lt(atVM *vm, atom_byte *p, atom_byte *end) {
	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_lt(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_lte(atVM *vm, atom_byte *p, atom_byte *end) {
	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_lte(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}

atom_byte* _at_executor_do_not(atVM *vm, atom_byte *p, atom_byte *end) {
	atValue operand;
	atValue result;

	// perform the operation
	if ( _at_executor_try_pop(vm, &operand, ATOM_TRUE) ) {
		at_value_from_number(&result, at_value_not(&operand));
	}
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operand
	at_value_dereference(&operand);

	return p;
}

atom_byte* _at_executor_do_and(atVM *vm, atom_byte *p, atom_byte *end) {
	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_and(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_or(atVM *vm, atom_byte *p, atom_byte *end) {
	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_or(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
atom_byte* _at_executor_do_xor(atVM *vm, atom_byte *p, atom_byte *end) {
	// get the operands
	atValue left, right;
	_at_executor_try_pop(vm, &right, ATOM_TRUE);
	_at_executor_try_pop(vm, &left, ATOM_TRUE);

	// determine whether the comparison is true or false
	atValue result;
	if ( at_value_xor(&left, &right) )
		at_value_from_number(&result, ATOM_TRUE);
	else
		at_value_from_number(&result, ATOM_FALSE);

	// push the result
	at_vm_push(vm, &result);

	// dereference the operands
	at_value_dereference(&left);
	at_value_dereference(&right);

	return p;
}
