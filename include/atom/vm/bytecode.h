/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: bytecode.h
 * @Last modified by:   mitchell
 * @Last modified time: 12/06/2018
 *
 * Contains bytecode listing and prototypes for functions
 * used for manipulating bytecode
 */

#ifndef ATOM_VM_BYTECODE_H
#define ATOM_VM_BYTECODE_H


#include "atom/types.h"


typedef enum _atBytecode {
	ATOM_BC_HALT,         // Suspends execution
	//ATOM_BC_TERMINATE,    // Immediately terminates the program, deallocating the VM

	ATOM_BC_PUSH_NULL,       // Loads a null value
	ATOM_BC_PUSH_NUMBER,     // Pushes a number onto the current call frame operand stack
	ATOM_BC_PUSH_STRING,     // Pushes a string onto the current call frame operand stack

	ATOM_BC_DISPLAY_VALUE,   // Prints a value to the command line. Takes a value from the operand stack


	ATOM_BC_LOAD_VALUE,      // Loads a value from the current call table. If the value is not found,
	                         // the global table is indexed. Null is pushed if it is still not found.
							 // Takes a string.
	ATOM_BC_STORE,           // Stores a value in a table. If the key is not found in the current call table,
	                         // the global table is indexed next. Takes a key and a value
	ATOM_BC_STORE_LOCAL,     // Stores a value into the current call frame. Takes a key and a value

	ATOM_BC_STORE_TABLE,     // Stores a value in a table. Takes a key and a table


	ATOM_BC_NEW_FUNCTION,    // Creates a new function, pushing it onto the call frame operand stack.
	                         // Takes a number of arguments, and the length in bytes of the function body.
	ATOM_BC_LOAD_ARGUMENT,   // Loads an argument to the current function. Takes an argument index.
	ATOM_BC_CALL,            // Calls a function on the top of the current call frame operand stack.
	                         // Takes a function, a number of arguments, and the arguments.
	ATOM_BC_RETURN,          // Exits the current call frame, pushing a value onto the next higher call frame.
	                         // Takes a value

	ATOM_BC_JUMP,            // Takes an offset to move execution to
	ATOM_BC_CJUMP,           // Conditional jump, takes a boolean on the op-stack and an offset to jump
							 // if the value is false in the bytecode. Execution is continued if the
							 // boolean value is true.



 	ATOM_BC_NEW_TABLE,       // Creates a new table value, pushing it onto the call frame operand stack.
	ATOM_BC_INDEX,           // Indexes a table. Takes a table and a string, both from the operand stack.
	                         // Pushes a value

	ATOM_BC_DEBUG_LINE,
	ATOM_BC_DEBUG_COLUMN,

	ATOM_BC_ADD,
	ATOM_BC_SUB,
	ATOM_BC_MUL,
	ATOM_BC_DIV,
	ATOM_BC_MOD,
	ATOM_BC_EXP,
	ATOM_BC_NEGATE, // negation
	ATOM_BC_FACT,   // factorial operation

	ATOM_BC_COMP_EQ,
	ATOM_BC_COMP_NEQ,
	ATOM_BC_COMP_GT,
	ATOM_BC_COMP_GTE,
	ATOM_BC_COMP_LT,
	ATOM_BC_COMP_LTE,

	ATOM_BC_NOT,
	ATOM_BC_AND,
	ATOM_BC_OR,
	ATOM_BC_XOR,


	ATOM_BC_COUNT, // The number of codes
} atBytecode;



/**
 * Gets the next byte from some bytecode
 * @param  bytecode  The byteocde
 * @param  offs      The offset of the bytecode
 * @param  bc_length The length of the bytecode
 * @param  out       Where to store the byte
 * @return           THe byte immediately following the byte, or NULL if an error occurred
 */
const atom_byte* at_bc_next_byte(const atom_byte *bytecode, unsigned offs, unsigned bc_length, atom_byte *out);

/**
 * Gets the next number from some bytecode
 * @param  bytecode  The bytecode
 * @param  offs      The offset of the bytecode
 * @param  bc_length The bytecode length
 * @param  out       Where to store the number
 * @return           The byte immediately following the number or NULL if an error occurred
 */
const atom_byte* at_bc_next_number(const atom_byte *bytecode, unsigned offs, unsigned bc_length, atom_number *out);

/**
 * Gets the next null-terminated string from some bytecode
 * @param  bytecode  The bytecode
 * @param  offs      The offset of the bytecode
 * @param  bc_length The bytecode length
 * @param  out       Where to store the string
 * @return           The byte immediately following the string or NULL if an error occurred
 */
const atom_byte* at_bc_next_string(const atom_byte *bytecode, unsigned offs, unsigned bc_length, char **out);



/**
 * Returns the name of a bytecode instruction
 * @param  code The code
 * @return      The name
 */
const char* at_bc_get_name(atom_byte code);



/**
 * Prints bytecode as text
 * @param bytecode The bytecode
 * @param length   The length
 */
void at_bc_print(atom_byte *bytecode, unsigned length);






#endif
