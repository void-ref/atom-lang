/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: bytecode.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/06/2018
 */


#include "atom/vm/bytecode.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>



/**
 * Gets the next byte from some bytecode
 * @param  bytecode  The byteocde
 * @param  offs      The offset of the bytecode
 * @param  bc_length The length of the bytecode
 * @param  out       Where to store the byte
 * @return           THe byte immediately following the byte, or NULL if an error occurred
 */
const atom_byte* at_bc_next_byte(const atom_byte *bytecode, unsigned offs, unsigned bc_length, atom_byte *out) {

	// check if the bytecode has sufficient memory left
	if ( offs + 1 > bc_length ) {
		printf("Unexpected end of bytecode reached; cannot read byte.\n");
		return NULL;
	}

	// get the byte
	*out = *(bytecode + offs);

	// return the new position
	return bytecode + offs + 1;
}

/**
 * Gets the next number from some bytecode
 * @param  bytecode  The bytecode
 * @param  offs      The offset of the bytecode
 * @param  bc_length The bytecode length
 * @param  out       Where to store the number
 * @return           The byte immediately following the number or NULL if an error occurred
 */
const atom_byte* at_bc_next_number(const atom_byte *bytecode, unsigned offs, unsigned bc_length, atom_number *out) {

	// check if the bytecode has sufficient memory left
	if ( offs + sizeof(atom_number) > bc_length ) {
		printf("Unexpected end of bytecode reached; cannot read number.\n");
		return NULL;
	}

	// get the number
	*out = *(atom_number*)(bytecode + offs);

	// return the new position
	return bytecode + offs + sizeof(atom_number);
}

/**
 * Gets the next null-terminated string from some bytecode
 * @param  bytecode  The bytecode
 * @param  offs      The offset of the bytecode
 * @param  bc_length The bytecode length
 * @param  out       Where to store the string
 * @return           The byte immediately following the string or NULL if an error occurred
 */
const atom_byte* at_bc_next_string(const atom_byte *bytecode, unsigned offs, unsigned bc_length, char **out) {

	// read characters until a null-terminator is reached
	const atom_byte *p = bytecode + offs;
	while ( p != bytecode + bc_length && *p != '\0' )
		++p;

	// check if the end was reached without a null-terminator
	if ( *p != '\0' ) {
		printf("Unexpected end of bytecode reached; cannot read string.\n");
		return NULL;
	}

	// set the output
	*out = bytecode + offs;


	// return the new position
	return p + 1;
}



/**
 * Returns the name of a bytecode instruction
 * @param  code The code
 * @return      The name
 */
const char* at_bc_get_name(atom_byte code) {

	switch ( code ) {
		case ATOM_BC_HALT:
			return "halt";

		case ATOM_BC_PUSH_NULL:
			return "push_null";
		case ATOM_BC_PUSH_NUMBER:
			return "push_number";
		case ATOM_BC_PUSH_STRING:
			return "push_string";

		case ATOM_BC_DISPLAY_VALUE:
			return "display_value";

		case ATOM_BC_LOAD_VALUE:
			return "load_value";
		case ATOM_BC_STORE:
			return "store";
		case ATOM_BC_STORE_LOCAL:
			return "store_local";
		case ATOM_BC_STORE_TABLE:
			return "store_table";

		case ATOM_BC_JUMP:
			return "jump";
		case ATOM_BC_CJUMP:
			return "cjump";

		case ATOM_BC_ADD:
			return "add";
		case ATOM_BC_SUB:
			return "sub";
		case ATOM_BC_MUL:
			return "mul";
		case ATOM_BC_DIV:
			return "div";
		case ATOM_BC_MOD:
			return "mod";
		case ATOM_BC_EXP:
			return "exp";
		case ATOM_BC_NEGATE:
			return "negate";
		case ATOM_BC_FACT:
			return "factorial";

		case ATOM_BC_NEW_FUNCTION:
			return "new_func";
		case ATOM_BC_LOAD_ARGUMENT:
			return "load_arg";
		case ATOM_BC_CALL:
			return "call";
		case ATOM_BC_RETURN:
			return "return";

		case ATOM_BC_INDEX:
			return "index";

		case ATOM_BC_COMP_EQ:
			return "ceq";
		case ATOM_BC_COMP_NEQ:
			return "cneq";
		case ATOM_BC_COMP_GT:
			return "cgt";
		case ATOM_BC_COMP_GTE:
			return "cgte";
		case ATOM_BC_COMP_LT:
			return "clt";
		case ATOM_BC_COMP_LTE:
			return "clte";

		case ATOM_BC_NOT:
			return "not";
		case ATOM_BC_AND:
			return "and";
		case ATOM_BC_OR:
			return "or";
		case ATOM_BC_XOR:
			return "xor";



		default:
			return "not implemented";
	}



}


const atom_byte* at_bc_print_helper(const atom_byte *bytecode, unsigned length, int indent);

/**
 * Prints bytecode as text
 * @param bytecode The bytecode
 * @param length   The length
 */
void at_bc_print(atom_byte *bytecode, unsigned length) {
	at_bc_print_helper(bytecode, length, 0);
}

/**
 * Prints bytecode as text
 * @param bytecode The bytecode
 * @param length   The length
 */
const atom_byte* at_bc_print_helper(const atom_byte *bytecode, unsigned length, int indent) {

	const atom_byte *p = bytecode;
	const atom_byte *end = bytecode + length;

	while ( p < bytecode + length ) {
		for (int i = 0; i <= indent; ++i)
			printf("\t");
		printf("%s", at_bc_get_name(*p));


		switch ( *p ) {
			case ATOM_BC_PUSH_NUMBER: {
				atom_number n;
				p = at_bc_next_number(p, 1, end - p, &n);
				printf(": %f", n);
				break;
			}
			case ATOM_BC_PUSH_STRING:
			case ATOM_BC_LOAD_VALUE:
			case ATOM_BC_STORE:
			case ATOM_BC_STORE_LOCAL: {
				char *s;
				p = at_bc_next_string(p, 1, end - p, &s);
				printf(": %s", s);
				break;
			}
			case ATOM_BC_CALL:
			case ATOM_BC_LOAD_ARGUMENT: {
				atom_byte index;
				p = at_bc_next_byte(p, 1, end - p, &index);
				printf(": %i", (int)index);
				break;
			}
			case ATOM_BC_NEW_FUNCTION: {
				printf(": \n");

				++p;
				atom_int func_length = *(atom_int*)p;
				p += sizeof(atom_int);
				p = at_bc_print_helper(p, func_length, indent + 1);
				break;
			}
			case ATOM_BC_JUMP: {
			case ATOM_BC_CJUMP:

				++p;
				atom_int offset = *(atom_int*)p;
				p += sizeof(atom_int);
				printf(": %d", offset);
				break;
			}
			default:
				++p;
		}



		printf("\n");


	}

	return p;
}
