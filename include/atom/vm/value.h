/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: value.h
 * @Last modified by:   mitchell
 * @Last modified time: 12/02/2018
 */


#ifndef ATOM_VM_VALUE_H
#define ATOM_VM_VALUE_H

#include "atom/types.h"

typedef struct _atVM atVM;
typedef struct _atTable atTable;
typedef struct _atFunction atFunction;

typedef unsigned (*atUserFunction)(atVM *vm, unsigned arg_count);

typedef enum _atValueType {
	ATOM_VALUE_NULL,
	ATOM_VALUE_NUMBER,
	ATOM_VALUE_STRING,
	ATOM_VALUE_TABLE,
	ATOM_VALUE_FUNCTION,
	ATOM_VALUE_USER_FUNCTION,
} atValueType;



typedef struct _atValue {
	atValueType type;
	void     *raw_value;
	unsigned *ref_count;
} atValue;



/**
 * Increases the reference count of a value
 * @param value The value
 * @return      The value
 */
atValue* at_value_reference(atValue *value);

/**
 * Decreases the reference count of a value
 * @param value The value
 * @return      The value, or NULL if the value was released
 */
atValue* at_value_dereference(atValue *value);


/**
 * Initializes a value from a number
 * @param  value The value
 * @param  num   The number
 * @return       The value
 */
atValue* at_value_from_number(atValue *value, atom_number num);

/**
 * Initializes a value from a string
 * @param  value The value
 * @param  str   The string
 * @return       The value
 */
atValue* at_value_from_string(atValue *value, const char *str);

/**
 * Initializes a value from a table
 * @param  value The value
 * @param  table The table
 * @return       The value
 */
atValue* at_value_from_table(atValue *value, atTable *table);

/**
 * Initializes a value from a function
 * @param  value The value
 * @param  func  The function
 * @return       The value
 */
atValue* at_value_from_function(atValue *value, atFunction *func);

/**
 * Initializes a value from a C-side function
 * @param  value The value
 * @param  func  The function pointer
 * @return       The value
 */
atValue* at_value_from_cfunction(atValue *value, atUserFunction func);

/**
 * Sets a value to NULL
 * @param  value The value
 * @return       The value
 */
atValue* at_value_set_null(atValue *value);

/**
 * Checks if a value is NULL
 * @param  value The value
 * @return       True if the value is NULL, false otherwise.
 */
atom_bool at_value_is_null(atValue *value);

/**
 * Checks if a value is a number
 * @param  value The value
 * @return       True if the value is a number
 */
atom_bool at_value_is_number(atValue *value);

/**
 * Checks if a value is a string
 * @param  value The value
 * @return       True if the value is a string
 */
atom_bool at_value_is_string(atValue *value);

/**
 * Initializes a value from a string without copying it
 * @param  value The value
 * @param  str   The string
 * @return       The value
 */
atValue* at_value_from_string_nocopy(atValue *value, char *str);

/**
 * Checks if a value is a function
 * @param  value The value
 * @return       True if the value is a function
 */
atom_bool at_value_is_function(atValue *value);


/**
 * Retrieves a numeric value from a generic value
 * @param  value The value
 * @return       The number
 */
atom_number at_value_to_number(atValue *value);



/**
 * Prints a value
 * @param value The value
 */
void at_value_print(atValue *value);

/**
 * Checks if two values are equal
 * @param  a The first value
 * @param  b The second value
 * @return   True if the values are equal
 */
atom_bool at_value_equals(atValue *a, atValue *b);

/**
 * Checks if two values are not equal
 * @param  a The first value
 * @param  b The second value
 * @return   True if the values are not equal
 */
atom_bool at_value_nequals(atValue *a, atValue *b);

/**
 * Checks if 'a' is greater than 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' > 'b'
 */
atom_bool at_value_gt(atValue *a, atValue *b);

/**
 * Checks if 'a' is greater than or equal to 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' >= 'b'
 */
atom_bool at_value_gte(atValue *a, atValue *b);

/**
 * Checks if 'a' is less than 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' < 'b'
 */
atom_bool at_value_lt(atValue *a, atValue *b);

/**
 * Checks if 'a' is less than or equal to 'b'
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' <= 'b'
 */
atom_bool at_value_lte(atValue *a, atValue *b);

/**
 * Performs the boolean not operation
 * @param  a The first value
 * @return   The negated value of 'a'
 */
atom_bool at_value_not(atValue *a);

/**
 * Performs the boolean and operation
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' and 'b'
 */
atom_bool at_value_and(atValue *a, atValue *b);

/**
 * Performs the boolean or operation
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' or 'b'
 */
atom_bool at_value_or(atValue *a, atValue *b);

/**
 * Performs the boolean exclusive or operation
 * @param  a The first value
 * @param  b The second value
 * @return   True if 'a' xor 'b'
 */
atom_bool at_value_xor(atValue *a, atValue *b);








#endif
