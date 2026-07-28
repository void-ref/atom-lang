/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: parser.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/02/2018
 */


#include "atom/parser/parser.h"
#include "atom/vm/bytecode.h"
#include "atom/util/memory_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define ATOM_PARSER_DEFAULT_BC_SIZE 128

/**
 * Parses an expression
 * @param  parser       The parser
 * @param  left_operand The starting token
 * @param  precedence   0 to parse the entire expression, 1 to parse only the next group/term,
 *                      or 2 to allow only table indexing
 * @return              The next token
 */
atToken _at_parser_do_expression(atParser *parser, atToken *start, unsigned precedence);


/**
 * Parses a function definition
 * @param  parser     The parser
 */
atToken _at_parser_do_function_def(atParser *parser);

/**
 * Parses a function call
 * @param  parser     The parser
 */
atToken _at_parser_do_function_call(atParser *parser, atToken *start);

/**
 * Converts a string to a number
 * @param  parser     The parser
 * @param  token      The token
 * @return            The number
 */
atom_number _at_parser_to_number(atToken *token);



/**
 * Signals that an error occurred
 * @param  parser The parser
 * @param  token  The token. The type is set to EOF
 * @param  msg    The error message
 * @return        A copy of the token
 */
atToken _at_parser_error(atParser *parser, atToken *token, const char *msg);



/**
 * Reserves an additional number of bytes in the bytecode,
 * resizing the bytecode array if necessary
 * @param parser The parser
 * @param size   The number of additional bytes
 */
void _at_parser_reserve(atParser *parser, unsigned size);

/**
 * Writes a byte to the parser bytecode
 * @param parser The parser
 * @param byte   The byte
 */
void _at_parser_put_byte(atParser *parser, atom_byte byte);

/**
 * Writes a number to the parser bytecode
 * @param parser The parser
 * @param num    The number
 */
void _at_parser_put_number(atParser *parser, atom_number num);

/**
 * Writes a string to the parser
 * @param parser The parser
 * @param s      The string
 * @param len    The length of the string
 */
void _at_parser_put_string(atParser *parser, const char *s, unsigned len);

/**
 * Writes a string token to the parser
 * @param parser The parser
 * @param str    The string token
 */
void _at_parser_put_string_token(atParser *parser, atToken *str);


atToken _at_parser_do_operator(atParser *parser, atToken *operator, atom_bool unary);
atToken _at_parser_do_group(atParser *parser, atToken *start);
atToken _at_parser_do_identifier(atParser *parser, atToken *start, atom_bool do_assignment);
atToken _at_parser_do_keyword(atParser *parser, atToken *start);
atToken _at_parser_do_if(atParser *parser, atToken *start);



/**
 * Initializes a parser with a source code string
 * @param parser The parser
 * @param str    The string
 * @return       The parser
 */
atParser* at_parser_init(atParser *parser, const char *str) {

	// intialize the scanner with the source code
	at_scanner_open(&parser->scanner, str);

	// initialize the bytecode array
	parser->bytecode          = malloc(ATOM_PARSER_DEFAULT_BC_SIZE);
	parser->bytecode_length   = 0;
	parser->bytecode_capacity = ATOM_PARSER_DEFAULT_BC_SIZE;
	parser->error_count       = 0;

	return parser;
}

/**
 * Frees resources associated with a parser.
 * @param parser The parser.
 */
void at_parser_free(atParser *parser) {

	// free resources used by the scanner
	at_scanner_close(&parser->scanner);

	// free the bytecode
	free(parser->bytecode);
	parser->bytecode_length   = 0;
	parser->bytecode_capacity = 0;
}



/**
 * Runs a parser and produces bytecode
 * @param parser The parser
 */
void at_parser_parse(atParser *parser) {


	atToken next_token;

	// Parse statements until the end is reached
	// while ( (next_token = _at_parser_do_statement(parser)).type != ATOM_TOKEN_EOF );


	at_scanner_next(&parser->scanner, &next_token);

	while ( next_token.type != ATOM_TOKEN_EOF ) {
		next_token = _at_parser_do_expression(parser, &next_token, 0);
		if ( next_token.type == ATOM_TOKEN_SEPARATOR && (next_token.string[0] == ';' || next_token.string[0] == '\n') )
			at_scanner_next(&parser->scanner, &next_token);
	}


	// insert the halt command
	_at_parser_put_byte(parser, ATOM_BC_HALT);

}



/**
 * Parses an expression
 * @param  parser       The parser
 * @param  left_operand The starting token
 * @param  precedence   0 to parse the entire expression, 1 to parse only the next group/term,
 *                      2 to end when an open parenthesis is found, or 3 for parameter expressions
 * @return              The next token
 */
atToken _at_parser_do_expression(atParser *parser, atToken *start, unsigned precedence) {

	// check if the end of the source is reached
	if ( start == NULL || start->type == ATOM_TOKEN_EOF ) {
		atToken end;
		end.type = ATOM_TOKEN_EOF;
		return end;
	}


	atToken token = *start;
	int counter = -1;

	while ( token.type != ATOM_TOKEN_EOF ) {

		// increase the loop counter
		++counter;

		// if immediate precedence is requested, and the first "thing" has been
		// parsed, we should stop parsing the expression here
		if ( precedence == 1 && counter == 1 ) {
			break;
		}

		// parse operators
		if ( token.type == ATOM_TOKEN_OPERATOR ) {
			token = _at_parser_do_operator(parser, &token, counter == 0);
		}
		// parse keywords
		else if ( token.type == ATOM_TOKEN_KEYWORD ) {
			// an 'end' token ends any expression
			if ( strncmp(token.string, "end", 3) == 0 ) {
				return token;
			}
			// an 'else' token ends any expression
			else if ( strncmp(token.string, "else", 4) == 0 ) {
				return token;
			}

			token = _at_parser_do_keyword(parser, &token);
		}
		// parse grouping
		else if ( token.type == ATOM_TOKEN_SEPARATOR ) {
			char separator = token.string[0];

			// handle separators
			switch ( separator ) {

				// end of expression
				case ';':
				case '\n':
					return token;

				// start grouped expression
				case '(':

					// check if the start parenthesis ends the expression
					if ( precedence == 2 ) {
						return token;
					}
					else {
						// get the token right after the '('
						at_scanner_next(&parser->scanner, &token);

						// start parsing an expression from right after the '('
						token = _at_parser_do_expression(parser, &token, 0);
					}

					break;

				// end grouped expression
				case ')':
					if ( precedence == 3 ) {
						return token;
					}
					else {
						// get the token following the ')'
						at_scanner_next(&parser->scanner, &token);
					}

					// stop parsing the expression
					return token;


				// function call
				case '@':
					// get the token right after the '@'
					at_scanner_next(&parser->scanner, &token);

					// parse the function call
					token = _at_parser_do_function_call(parser, &token);
					break;

				// comma operator
				case ',':
					// scan the token right after the comma
					at_scanner_next(&parser->scanner, &token);

					// stop parsing the expression
					return token;

				default:
					return _at_parser_error(parser, &token, "Invalid token in expression");
			}
		}
		// parse identifiers
		else if ( token.type == ATOM_TOKEN_IDENTIFIER ) {
			// parse the identifier, allowing for assignment only if low
			// precedence is requested
			token = _at_parser_do_identifier(parser, &token, precedence == 0);
		}
		// parse numbers
		else if ( token.type == ATOM_TOKEN_NUMBER ) {

			// write the instruction to push a number
			_at_parser_put_byte(parser, ATOM_BC_PUSH_NUMBER);
			_at_parser_put_number(parser, _at_parser_to_number(&token));

			// scan the next token
			at_scanner_next(&parser->scanner, &token);
		}
		// parse strings
		else if ( token.type == ATOM_TOKEN_STRING ) {

			// write the instruction to push a string
			_at_parser_put_byte(parser, ATOM_BC_PUSH_STRING);
			_at_parser_put_string_token(parser, &token);

			// scan the next token
			at_scanner_next(&parser->scanner, &token);
		}
		else {
			return _at_parser_error(parser, &token, "Unexpected token in expression");
		}


	}

	return token;
}

atToken _at_parser_do_identifier(atParser *parser, atToken *identifier, atom_bool do_assignment) {

	// scan the token immediately after the identifier
	atToken next;
	at_scanner_next(&parser->scanner, &next);

	// check if something is being stored here (and is allowed)
	if ( do_assignment && next.type == ATOM_TOKEN_OPERATOR && at_token_operator_type(&next) == ATOM_OPERATOR_ASSIGN ) {

		// scan the token following the equals sign
		at_scanner_next(&parser->scanner, &next);

		// parse the expression to the right of the equals sign
		next = _at_parser_do_expression(parser, &next, 0);

		// put a store instruction
		_at_parser_put_byte(parser, ATOM_BC_STORE);
		_at_parser_put_string_token(parser, identifier);
	}
	else if ( next.type == ATOM_TOKEN_OPERATOR && at_token_operator_type(&next) == ATOM_OPERATOR_INDEX ) {
		// put a load instruction
		_at_parser_put_byte(parser, ATOM_BC_LOAD_VALUE);
		_at_parser_put_string_token(parser, identifier);


		next = _at_parser_do_operator(parser, &next, ATOM_FALSE);
	}
	// a variable is being loaded
	else {
		// put a load instruction
		_at_parser_put_byte(parser, ATOM_BC_LOAD_VALUE);
		_at_parser_put_string_token(parser, identifier);
	}


	return next;
}

atToken _at_parser_do_operator(atParser *parser, atToken *operator, atom_bool unary) {

	// get the operator type
	unsigned type = at_token_operator_type(operator);

	// scan the token immediately after the operator
	atToken next;
	at_scanner_next(&parser->scanner, &next);

	// determine what to do based on the operator type
	switch ( type ) {

		// addition
		case ATOM_OPERATOR_ADD:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_ADD);
			break;

		// subtraction or negation
		case ATOM_OPERATOR_SUB:
			// handle unary subtraction (negation)
			if ( unary ) {
				next = _at_parser_do_expression(parser, &next, 1);
				_at_parser_put_byte(parser, ATOM_BC_NEGATE);
			}
			// handle binary subtraction
			else {
				next = _at_parser_do_expression(parser, &next, 0);
				_at_parser_put_byte(parser, ATOM_BC_SUB);
			}
			break;

		// multiplication
		case ATOM_OPERATOR_MUL:
			next = _at_parser_do_expression(parser, &next, 1);
			_at_parser_put_byte(parser, ATOM_BC_MUL);
			break;

		// division
		case ATOM_OPERATOR_DIV:
			next = _at_parser_do_expression(parser, &next, 1);
			_at_parser_put_byte(parser, ATOM_BC_DIV);
			break;

		// modulus
		case ATOM_OPERATOR_MOD:
			next = _at_parser_do_expression(parser, &next, 1);
			_at_parser_put_byte(parser, ATOM_BC_MOD);
			break;

		// power
		case ATOM_OPERATOR_EXP:
			next = _at_parser_do_expression(parser, &next, 1);
			_at_parser_put_byte(parser, ATOM_BC_EXP);
			break;

		// factorial
		case ATOM_OPERATOR_FACT:
			_at_parser_put_byte(parser, ATOM_BC_FACT);
			break;

		// not
		case ATOM_OPERATOR_NOT:
			next = _at_parser_do_expression(parser, &next, 1);
			_at_parser_put_byte(parser, ATOM_BC_NOT);
			break;

		// equals
		case ATOM_OPERATOR_EQS:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_COMP_EQ);
			break;

		// not equals
		case ATOM_OPERATOR_NEQS:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_COMP_NEQ);
			break;

		// greater than
		case ATOM_OPERATOR_GT:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_COMP_GT);
			break;

		// greater than or equal to
		case ATOM_OPERATOR_GTE:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_COMP_GTE);
			break;

		// less than
		case ATOM_OPERATOR_LT:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_COMP_LT);
			break;

		// less than or equal to
		case ATOM_OPERATOR_LTE:
			next = _at_parser_do_expression(parser, &next, 0);
			_at_parser_put_byte(parser, ATOM_BC_COMP_LTE);
			break;

		// indexing
		case ATOM_OPERATOR_INDEX:

			// parse the identifier to the right of the index operator ('.')
			if ( next.type == ATOM_TOKEN_IDENTIFIER ) {
				_at_parser_put_byte(parser, ATOM_BC_PUSH_STRING);
				_at_parser_put_string_token(parser, &next);
				at_scanner_next(&parser->scanner, &next);
			}
			else
				return _at_parser_error(parser, &next, "Error: table key must be an identifier");


			// check if something is being stored here
			if ( next.type == ATOM_TOKEN_OPERATOR && at_token_operator_type(&next) == ATOM_OPERATOR_ASSIGN ) {
				// get the first token to the right of the assignment operator ('=')
				at_scanner_next(&parser->scanner, &next);

				// parse the expression starting right after the assignment operator
				next = _at_parser_do_expression(parser, &next, 0);
				_at_parser_put_byte(parser, ATOM_BC_STORE_TABLE);
			}
			// it's cool, just indexing a (possible) table
			else
				_at_parser_put_byte(parser, ATOM_BC_INDEX);

			break;

		default:
			return _at_parser_error(parser, operator, "Invalid operation");
	}

	return next;
}

atToken _at_parser_do_if(atParser *parser, atToken *start) {

	// make sure the if statement starts with an open parenthesis
	if ( !(start->type == ATOM_TOKEN_SEPARATOR && start->string[0] == '(') ) {
		return _at_parser_error(parser, start, "Expect '(' to start condition");
	}


	// parse the condition
	atToken next = _at_parser_do_expression(parser, start, 1);

	// write the jump command


	// save four bytes for the jump offset (set this later)
	_at_parser_put_byte(parser, ATOM_BC_CJUMP);
	_at_parser_put_byte(parser, 0);
	_at_parser_put_byte(parser, 0);
	_at_parser_put_byte(parser, 0);
	_at_parser_put_byte(parser, 0);

	unsigned jump_offset_index = parser->bytecode_length;

	// holds a flag indicating if there is an else block
	atom_bool has_else = ATOM_FALSE;

	// parse a block of code
	while ( ATOM_TRUE ) {

		// parse statements
		next = _at_parser_do_expression(parser, &next, 0);

		// check if the end of the file was reached
		if ( next.type == ATOM_TOKEN_EOF ) {
			return _at_parser_error(parser, &next, "Unexpected end of file in conditional block");
		}
		// check if the 'end' keyword was returned
		else if ( next.type == ATOM_TOKEN_KEYWORD && strncmp(next.string, "end", 3) == 0 ) {
			break;
		}
		// check if the 'else' keyword was returned
		else if ( next.type == ATOM_TOKEN_KEYWORD && strncmp(next.string, "else", 4) == 0 ) {
			has_else = ATOM_TRUE;
			break;
		}
		// check if it is a terminating character
		else if ( next.type == ATOM_TOKEN_SEPARATOR && (next.string[0] == ';' || next.string[0] == '\n') ) {
			at_scanner_next(&parser->scanner, &next);
		}
	}

	at_scanner_next(&parser->scanner, &next);

	// save four bytes another jump offset - breaking out of the if statement
	// if there is an else statement after
	unsigned jump_offset2_index = parser->bytecode_length;


	// handle else statements
	if ( has_else ) {
		// write the jump command to jump out of the end of the if block
		_at_parser_put_byte(parser, ATOM_BC_JUMP);
		_at_parser_put_byte(parser, 0);
		_at_parser_put_byte(parser, 0);
		_at_parser_put_byte(parser, 0);
		_at_parser_put_byte(parser, 0);

		jump_offset2_index = parser->bytecode_length;

		// go back and write how far it is to jump from the if statement to the
		// end of the if statement
		atom_int *jump_offset_i = (atom_int*)(parser->bytecode + jump_offset_index - 4);
		*jump_offset_i = jump_offset2_index - jump_offset_index;



		// parse a block of code
		while ( ATOM_TRUE ) {

			// parse statements
			next = _at_parser_do_expression(parser, &next, 0);


			// check if the end of the file was reached
			if ( next.type == ATOM_TOKEN_EOF ) {
				return _at_parser_error(parser, &next, "Unexpected end of file in conditional block");
			}
			// check if the 'end' keyword was returned
			else if ( next.type == ATOM_TOKEN_KEYWORD && strncmp(next.string, "end", 3) == 0 ) {
				break;
			}
			// check if it is a terminating character
			else if ( next.type == ATOM_TOKEN_SEPARATOR && (next.string[0] == ';' || next.string[0] == '\n') ) {
				at_scanner_next(&parser->scanner, &next);
			}
		}

		// move past the last keyword
		//at_scanner_next(&parser->scanner, &next);

		// this is now the end of the conditional statement
		unsigned end_index = parser->bytecode_length;




		// go back and write how far it is to jump from the end of the if block to the
		// end of the else block
		atom_int *jump_offset2_i = (atom_int*)(parser->bytecode + jump_offset2_index - 4);
		*jump_offset2_i = end_index - jump_offset2_index ;
	}
	else {
		// go back and write how far it is to jump from the if statement to the
		// end of the if statement
		atom_int *jump_offset_i = (atom_int*)(parser->bytecode + jump_offset_index - 4);
		*jump_offset_i = jump_offset2_index - jump_offset_index;
	}


	return next;
}

atToken _at_parser_do_keyword(atParser *parser, atToken *start) {

	atToken next;

	// get the token right after the keyword
	at_scanner_next(&parser->scanner, &next);

	// check the keyword type
	if ( strncmp(start->string, "null", 4) == 0 ) {
		// push a null value
		_at_parser_put_byte(parser, ATOM_BC_PUSH_NULL);
	}
	else if ( strncmp(start->string, "if", 2) == 0 ) {
		// parse the if statement starting after the 'if' keyword
		next = _at_parser_do_if(parser, &next);

		// move past the end keyword
		at_scanner_next(&parser->scanner, &next);
	}
	else if ( strncmp(start->string, "and", 3) == 0 ) {

		// parse the expression
		next = _at_parser_do_expression(parser, &next, 0);

		// push the 'and' command
		_at_parser_put_byte(parser, ATOM_BC_AND);
	}
	else if ( strncmp(start->string, "or", 2) == 0 ) {

		// parse the expression
		next = _at_parser_do_expression(parser, &next, 0);

		// push the 'and' command
		_at_parser_put_byte(parser, ATOM_BC_OR);
	}
	else {
		return _at_parser_error(parser, &next, "Invalid token in expression");
	}




	return next;
}

/**
 * Parses a function call
 * @param  parser     The parser
 */
atToken _at_parser_do_function_call(atParser *parser, atToken *identifier) {

	// get the token following the identifier
	atToken next;
	// at_scanner_next(&parser->scanner, &next);

	// parse the thing we are calling
	next = _at_parser_do_identifier(parser, identifier, ATOM_FALSE);

	// make sure the next token is an open parenthesis
	if ( !(next.type == ATOM_TOKEN_SEPARATOR && next.string[0] == '(') ) {
		return _at_parser_error(parser, &next, "Expected '(' to start argument list");
	}

	// move past the open parenthesis
	at_scanner_next(&parser->scanner, &next);



	// Parse parameters
	int arg_count = 0;

	while ( ATOM_TRUE ) {

		// check if end of the parameter list was found
		if ( next.type == ATOM_TOKEN_SEPARATOR ) {
			if ( next.string[0] == ')' || next.string[0] == ';' || next.string[0] == '\n' )
				break;
		}
		else if ( next.type == ATOM_TOKEN_EOF ) {
			break;
		}

		// parse the next parameter with precedence 3 (parameter expression)
		next = _at_parser_do_expression(parser, &next, 3);
		++arg_count;
	}

	// put the call instruction
	_at_parser_put_byte(parser, ATOM_BC_CALL);
	_at_parser_put_byte(parser, (atom_byte)arg_count);


	// move past the close parenthesis
	at_scanner_next(&parser->scanner, &next);

	return next;
}


/**
 * Signals that an error occurred
 * @param  token The token. The type is set to EOF
 * @param  msg   The error message
 * @return       A copy of the token
 */
atToken _at_parser_error(atParser *parser, atToken *token, const char *msg) {
	parser->error_count++;

	// print the error message
	printf("%s: ", msg);
	at_token_print(token);

	// copy the token and set it to EOF
	atToken copy = *token;
	copy.type = ATOM_TOKEN_EOF;

	return copy;
}

/**
 * Reserves an additional number of bytes in the bytecode,
 * resizing the bytecode array if necessary
 * @param parser The parser
 * @param size   The number of additional bytes
 */
void _at_parser_reserve(atParser *parser, unsigned size) {

	// resize the bytecode array if it is not large enough
	if ( parser->bytecode_length + size > parser->bytecode_capacity ) {
		// copy the bytecode to a new array
		unsigned new_capacity = parser->bytecode_capacity * 2;
		atom_byte *new_arr = at_array_copy(parser->bytecode, parser->bytecode_length, new_capacity);
		free(parser->bytecode);

		// swap the bytecode array with the new one
		parser->bytecode = new_arr;
		parser->bytecode_capacity = new_capacity;
	}
}

/**
 * Writes a byte to the parser bytecode
 * @param parser The parser
 * @param byte   The byte
 */
void _at_parser_put_byte(atParser *parser, atom_byte byte) {

	// make sure there is room for the byte
	_at_parser_reserve(parser, 1);

	// store the byte
	parser->bytecode[parser->bytecode_length++] = byte;
}

/**
 * Writes a number to the parser bytecode
 * @param parser The parser
 * @param num    The number
 */
void _at_parser_put_number(atParser *parser, atom_number num) {

		// make sure there is room for the number
		_at_parser_reserve(parser, sizeof(num));

		// store the number
		*(atom_number*)(parser->bytecode + parser->bytecode_length) = num;
		parser->bytecode_length += sizeof(num);
}

/**
 * Writes a string to the parser
 * @param parser The parser
 * @param s      The string
 * @param len    The length of the string
 */
void _at_parser_put_string(atParser *parser, const char *s, unsigned len) {

	// reserve space for the string, including the null terminator
	_at_parser_reserve(parser, len + 1);

	// store the string
	for (int i = 0; i < len; ++i) {
		parser->bytecode[parser->bytecode_length++] = s[i];
	}

	// insert the null terminator
	parser->bytecode[parser->bytecode_length++] = '\0';

}

/**
 * Writes a string token to the parser
 * @param parser The parser
 * @param str    The string token
 */
void _at_parser_put_string_token(atParser *parser, atToken *str) {
	int len = str->length;

	// reserve space for the string
	_at_parser_reserve(parser, len + 1);


	char c;

	// store the string
	// TODO: escape characters
	for (int i = 0; i < len; ++i) {
		c = str->string[i];
		parser->bytecode[parser->bytecode_length++] = (atom_byte)c;
	}

	// insert the null terminator
	parser->bytecode[parser->bytecode_length++] = '\0';
}

/**
 * Converts a string to a number
 * @param  token      The token
 * @return            The number
 */
atom_number _at_parser_to_number(atToken *token) {
	char buff[128];
	memcpy(buff, token->string, token->length);
	buff[token->length] = '\0';
	return atof(buff);
}
