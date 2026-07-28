/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: value.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/02/2018
 */



#include "atom/vm/value.h"
#include "atom/vm/table.h"
#include "atom/vm/function.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deletes the data associated with a given value
 * @param value The value
 */
void at_value_delete(atValue *value);


/**
 * Increases the reference count of a value
 * @param value The value
 * @return      The value
 */
atValue* at_value_reference(atValue *value) {
	if ( value != NULL && value->type != ATOM_VALUE_NULL )
		++(*value->ref_count);
	return value;
}

/**
 * Decreases the reference count of a value
 * @param value The value
 * @return      The value, or NULL if the value was released
 */
atValue* at_value_dereference(atValue *value) {
	if ( value == NULL || value->type == ATOM_VALUE_NULL )
		return value;

	// delete the value once the number of references is zero
	if ( --(*value->ref_count) == 0 ) {
		at_value_delete(value);
		return NULL;
	}


	return value;
}



/**
 * Deletes the data associated with a given value
 * @param value The value
 */
void at_value_delete(atValue *value) {

	// do any special deleting first
	switch ( value->type ) {
		case ATOM_VALUE_TABLE:
			at_table_free((atTable*)value->raw_value);
			break;

		case ATOM_VALUE_FUNCTION:
			at_function_free((atFunction*)value->raw_value);
			break;

		case ATOM_VALUE_NUMBER:
		case ATOM_VALUE_STRING:
			break;

		default:
			return;
	}

	// actually free the raw pointers here
	free(value->raw_value);
	free(value->ref_count);
}





/**
 * Initializes a value from a number
 * @param  value The value
 * @param  num   The number
 * @return       The value
 */
atValue* at_value_from_number(atValue *value, atom_number num) {

	value->type = ATOM_VALUE_NUMBER;

	// allocate a raw value
	value->raw_value  = malloc(sizeof(num));
	*(atom_number*)value->raw_value = num;

	// allocate a reference count
	value->ref_count  = malloc(sizeof(unsigned int));
	*value->ref_count = 0;

	return value;
}

/**
 * Initializes a value from a string
 * @param  value The value
 * @param  str   The string
 * @return       The value
 */
atValue* at_value_from_string(atValue *value, const char *str) {
	value->type = ATOM_VALUE_STRING;

	// allocate a raw value
	int str_len = strlen(str);
	value->raw_value  = malloc(str_len + 1);
	strcpy((char*)value->raw_value, str);

	// allocate a reference count
	value->ref_count  = malloc(sizeof(unsigned int));
	*value->ref_count = 0;

	return value;
}

/**
 * Initializes a value from a string without copying it
 * @param  value The value
 * @param  str   The string
 * @return       The value
 */
atValue* at_value_from_string_nocopy(atValue *value, char *str) {
	value->type = ATOM_VALUE_STRING;

	// allocate a raw value
	value->raw_value  = str;

	// allocate a reference count
	value->ref_count  = malloc(sizeof(unsigned int));
	*value->ref_count = 0;

	return value;
}

/**
 * Initializes a value from a table
 * @param  value The value
 * @param  table The table
 * @return       The value
 */
atValue* at_value_from_table(atValue *value, atTable *table) {

	value->type = ATOM_VALUE_TABLE;

	// allocate a raw value
	value->raw_value = malloc(sizeof(atTable));

	atTable *table_value = (atTable*)value->raw_value;
	*table_value = *table;

	// allocate a reference count
	value->ref_count  = malloc(sizeof(unsigned int));
	*value->ref_count = 0;

	return value;
}

/**
 * Initializes a value from a function
 * @param  value The value
 * @param  func  The function
 * @return       The value
 */
atValue* at_value_from_function(atValue *value, atFunction *func) {
	value->type = ATOM_VALUE_FUNCTION;

	// allocate a raw value
	value->raw_value = malloc(sizeof(atFunction));

	atFunction *raw = (atFunction*)value->raw_value;
	*raw = *func;

	// allocate a reference count
	value->ref_count  = malloc(sizeof(unsigned int));
	*value->ref_count = 0;

	return value;
}

/**
 * Initializes a value from a C-side function
 * @param  value The value
 * @param  func  The function pointer
 * @return       The value
 */
atValue* at_value_from_cfunction(atValue *value, atUserFunction func) {
	value->type = ATOM_VALUE_USER_FUNCTION;
	value->raw_value = func;

	// allocate a reference count
	value->ref_count  = malloc(sizeof(unsigned int));
	*value->ref_count = 0;

	return value;
}

/**
 * Sets a value to NULL
 * @param  value The value
 * @return       The value
 */
atValue* at_value_set_null(atValue *value) {
	value->type = ATOM_VALUE_NULL;
	value->raw_value = NULL;
	value->ref_count = NULL;

	return value;
}

/**
 * Checks if a value is NULL
 * @param  value The value
 * @return       True if the value is NULL, false otherwise.
 */
atom_bool at_value_is_null(atValue *value) {
	return value == NULL || value->raw_value == NULL || value->type == ATOM_VALUE_NULL;
}

/**
 * Checks if a value is a number
 * @param  value The value
 * @return       True if the value is a number
 */
atom_bool at_value_is_number(atValue *value) {
	return value != NULL && value->type == ATOM_VALUE_NUMBER;
}

/**
 * Checks if a value is a string
 * @param  value The value
 * @return       True if the value is a string
 */
atom_bool at_value_is_string(atValue *value) {
	return value != NULL && value->type == ATOM_VALUE_STRING;
}

/**
 * Checks if a value is a function
 * @param  value The value
 * @return       True if the value is a function
 */
atom_bool at_value_is_function(atValue *value) {
	return value != NULL && value->type == ATOM_VALUE_FUNCTION;
}


/**
 * Retrieves a numeric value from a generic value
 * @param  value The value
 * @return       The number
 */
atom_number at_value_to_number(atValue *value) {
	if ( value == NULL || value->type == ATOM_VALUE_NULL )
		return 0;
	else
		return *(atom_number*)value->raw_value;
}


/**
 * Prints a value
 * @param value The value
 */
void at_value_print(atValue *value) {

	if ( value == NULL || value->type == ATOM_VALUE_NULL ) {
		printf("null\n");
		return;
	}


	switch ( value->type ) {
		case ATOM_VALUE_NUMBER:
			printf("%f\n", *(atom_number*)value->raw_value);
			break;
		case ATOM_VALUE_STRING:
			printf("'%s'\n", (char*)value->raw_value);
			break;
		case ATOM_VALUE_TABLE:
			printf("table %p\n", value->raw_value);
			break;
		case ATOM_VALUE_FUNCTION:
			printf("function %p\n", value->raw_value);
			break;
		case ATOM_VALUE_USER_FUNCTION:
			printf("user function %p\n", value->raw_value);
			break;
		default:
			printf("invalid type\n");
	}


}




/**
 * Checks if two values are equal
 * @param  a The first value
 * @param  b The second value
 * @return   True if the values are equal
 */
atom_bool at_value_equals(atValue *a, atValue *b) {

	// different types are never equal
	if ( a->type != b->type )
		return ATOM_FALSE;
	// two null values are always equal
	else if ( at_value_is_null(a) && at_value_is_null(b) )
		return ATOM_TRUE;

	// compare numeric types
	if ( a->type == ATOM_VALUE_NUMBER ) {
		atom_number a_num = *(atom_number*)a->raw_value;
		atom_number b_num = *(atom_number*)b->raw_value;

		return a_num == b_num;
	}
	// compare string types
	else if ( a->type == ATOM_VALUE_STRING ) {
		char *a_str = (char*)a->raw_value;
		char *b_str = (char*)b->raw_value;

		return strcmp(a_str, b_str) == 0;
	}
	// for non-primitive types, just compare pointers
	else {
		return a->raw_value == b->raw_value;
	}
}

/**
 * Checks if two values are not equal
 * @param  a The first value
 * @param  b The second value
 * @return   True if the values are not equal
 */
atom_bool at_value_nequals(atValue *a, atValue *b) {
	return !at_value_equals(a, b);
}

/**
 * Checks if 'a' is greater than 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' > 'b'
 */
atom_bool at_value_gt(atValue *a, atValue *b) {

	// different types cannot be compared
	if ( a->type != b->type )
		return ATOM_FALSE;
	// two null values cannot be compared
	else if ( at_value_is_null(a) && at_value_is_null(b) )
		return ATOM_FALSE;

	// compare numeric types
	if ( a->type == ATOM_VALUE_NUMBER ) {
		atom_number a_num = *(atom_number*)a->raw_value;
		atom_number b_num = *(atom_number*)b->raw_value;

		return a_num > b_num;
	}
	// compare string types
	else if ( a->type == ATOM_VALUE_STRING ) {
		// compare lengths for strings
		unsigned a_len = strlen((char*)a->raw_value);
		unsigned b_len = strlen((char*)b->raw_value);

		return a_len > b_len;
	}
	// cannot do comparison of non-primitive values
	else {
		return ATOM_FALSE;
	}
}

/**
 * Checks if 'a' is greater than or equal to 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' >= 'b'
 */
atom_bool at_value_gte(atValue *a, atValue *b) {
	// different types cannot be compared
	if ( a->type != b->type )
		return ATOM_FALSE;
	// two null values cannot be compared
	else if ( at_value_is_null(a) && at_value_is_null(b) )
		return ATOM_FALSE;

	// compare numeric types
	if ( a->type == ATOM_VALUE_NUMBER ) {
		atom_number a_num = *(atom_number*)a->raw_value;
		atom_number b_num = *(atom_number*)b->raw_value;

		return a_num >= b_num;
	}
	// compare string types
	else if ( a->type == ATOM_VALUE_STRING ) {
		// compare lengths for strings
		unsigned a_len = strlen((char*)a->raw_value);
		unsigned b_len = strlen((char*)b->raw_value);

		return a_len >= b_len;
	}
	// cannot do comparison of non-primitive values
	else {
		return ATOM_FALSE;
	}
}

/**
 * Checks if 'a' is less than 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' < 'b'
 */
atom_bool at_value_lt(atValue *a, atValue *b) {
	return at_value_gt(b, a);
}

/**
 * Checks if 'a' is less than or equal to 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' <= 'b'
 */
atom_bool at_value_lte(atValue *a, atValue *b) {
	return at_value_gte(b, a);
}

/**
 * Performs the boolean not operation
 * @param  a The first value
 * @return   The negated value of 'a'
 */
atom_bool at_value_not(atValue *a) {
	if ( a->type == ATOM_VALUE_NUMBER ) {
		atom_number a_num = *(atom_number*)a->raw_value;
		return a_num == ATOM_FALSE;
	}
	else
		return ATOM_FALSE;
}


/**
 * Performs the boolean and operation
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' and 'b'
 */
atom_bool at_value_and(atValue *a, atValue *b) {
	// make sure both types are numbers
	if ( a->type == ATOM_VALUE_NUMBER && b->type == ATOM_VALUE_NUMBER ) {
		atom_number a_num = *(atom_number*)a->raw_value;
		atom_number b_num = *(atom_number*)b->raw_value;
		return a_num == ATOM_TRUE && b_num == ATOM_TRUE;
	}
	// non-numeric types cannot be compared
	else {
		return ATOM_FALSE;
	}
}

/**
 * Performs the boolean or operation
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' or 'b'
 */
atom_bool at_value_or(atValue *a, atValue *b) {
	// make sure both types are numbers
	if ( a->type == ATOM_VALUE_NUMBER && b->type == ATOM_VALUE_NUMBER ) {
		atom_number a_num = *(atom_number*)a->raw_value;
		atom_number b_num = *(atom_number*)b->raw_value;
		return a_num == ATOM_TRUE || b_num == ATOM_TRUE;
	}
	// non-numeric types cannot be compared
	else {
		return ATOM_FALSE;
	}
}

/**
 * Performs the boolean exclusive or operation
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' xor 'b'
 */
atom_bool at_value_xor(atValue *a, atValue *b) {
	// make sure both types are numbers
	if ( a->type == ATOM_VALUE_NUMBER && b->type == ATOM_VALUE_NUMBER ) {
		int ai = (int)*(atom_number*)a->raw_value;
		int bi = (int)*(atom_number*)b->raw_value;
		return !ai != !bi;
	}
	// non-numeric types cannot be compared
	else {
		return ATOM_FALSE;
	}
}
