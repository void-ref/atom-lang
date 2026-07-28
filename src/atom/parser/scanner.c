/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: scanner.c
 * @Last modified by:   mitchell
 * @Last modified time: 11/30/2018
 */

#include "atom/parser/scanner.h"
#include "atom/parser/lang.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Gets the next character in a scanner, incrementing the position and updating line/column counts
 * @param  scanner The scanner
 * @return         The next character, or 0 if EOF is reached
 */
char _at_scanner_next_char(atScanner *scanner);

/**
 * Gets the next character in a scanner without incrementing the position
 * @param  scanner The scanner
 * @return         The next character, or 0 if the scanner is at EOF
 */
char _at_scanner_peek_char(atScanner *scanner);

/**
 * Skips any spaces (or comments, but not '\n' or '\t') in the scanner, setting scanner->p to the
 * next non-space character
 * @param scanner The scanner
 */
void _at_scanner_skip_space(atScanner *scanner);

/**
 * Scans the next word token
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_word(atScanner *scanner, atToken *out);

/**
 * Scans the next number token
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_number(atScanner *scanner, atToken *out);

/**
 * Scans the next symbol token (includes '\n' and '\t')
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_symbol(atScanner *scanner, atToken *out);


/**
 * Scans the next string token inside of quotation marks
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_string(atScanner *scanner, atToken *out);

/**
 * Sets a token to EOF
 * @param scanner The scanner
 * @param token   The token
 */
void _at_scanner_set_eof(atScanner *scanner, atToken *token);

/**
 * Gets the number of characters left in the scanner (including the current one)
 * @param scanner The scanner
 */
unsigned int _at_scanner_chars_left(atScanner *scanner);





/**
 * Initializes a scanner
 * @param  scanner       A pointer to the scanner to initialize
 * @param  source        The source string (this is copied)
 * @return               A pointer to 'scanner'
 */
atScanner* at_scanner_open(atScanner *scanner, const char *source) {

	// copy the source to the scanner
	scanner->length = strlen(source);
	scanner->source = (char*)malloc(scanner->length + 1);
	strcpy(scanner->source, source);

	// set the starting pointer to the source pointer
	scanner->p = scanner->source;

	// misc
	scanner->line = 1;
	scanner->column = 1;


	return scanner;
}


/**
 * Frees resources used by the scanner
 * @param scanner A pointer to the scanner
 */
void at_scanner_close(atScanner *scanner) {

	// free the source code
	free(scanner->source);

	// reset the scanner
	scanner->length = 0;
	scanner->source = 0;
	scanner->p = NULL;
}

/**
 * Checks if the scanner has reached the end
 * @param  scanner the scanner
 * @return         ATOM_TRUE if the scanner has reached EOF, ATOM_FALSE otherwise
 */
atom_bool at_scanner_is_eof(atScanner *scanner) {
	return (scanner->p - scanner->source) >= scanner->length;
}


/**
 * Retrieves the next token in the scanner
 * @param  scanner A pointer to the scanner
 * @param  out     A pointer to token output
 * @return         The number of characters read or -1 if EOF is reached
 */
int at_scanner_next(atScanner *scanner, atToken *out) {

	// store the inital position for later
	char *init_p = scanner->p;

	// skips any space characters (only spaces, not '\t' or '\n')
	_at_scanner_skip_space(scanner);

	// check if the scanner has reached the end of the file
	if ( at_scanner_is_eof(scanner) ) {
		// set the token to EOF
		_at_scanner_set_eof(scanner, out);

		// signal that the end is reached
		return -1;
	}

	char c = *scanner->p;

	// scan a number
	if ( isdigit(c) )
		_at_scanner_next_number(scanner, out);

	// scan a word
	else if ( isalpha(c) || c == '_' )
		_at_scanner_next_word(scanner, out);

	// scan a string
	else if ( c == '\"' )
		_at_scanner_next_string(scanner, out);

	// scan a symbol
	else
		_at_scanner_next_symbol(scanner, out);


	// return the number of characters moved ahead
	return scanner->p - init_p;
}



/**
 * Skips any spaces (or comments, but not '\n' or '\t') in the scanner, setting scanner->p to the
 * next non-space character
 * @param scanner The scanner
 */
void _at_scanner_skip_space(atScanner *scanner) {

	// cache the scanner state
	int chars_left = _at_scanner_chars_left(scanner);
	char c;


	while ( chars_left > 0 ) {
		c = _at_scanner_peek_char(scanner);

		// check if a comment has been reached
		if ( c == ATOM_LANG_COMMENT_CHAR ) {
			// read characters until the next line is reached
			while ( chars_left-- >= 0 && _at_scanner_next_char(scanner) != '\n' );
		}
		// check if a non-space character has been reached
		else if ( c != ' ' && c != '\t' )
			break;
		// move the scanner ahead
		else {
			_at_scanner_next_char(scanner);
			--chars_left;
		}
	}
}

/**
 * Scans the next word token
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_word(atScanner *scanner, atToken *out) {

	// cache the scanner state
	char *start         = scanner->p;
	unsigned int line   = scanner->line;
	unsigned int column = scanner->column;

	int chars_left  = _at_scanner_chars_left(scanner);
	char c;

	// Move the scanner position until a non-alphanumeric character is reached
	while ( chars_left > 0 ) {
		c = _at_scanner_peek_char(scanner);
		--chars_left;

		// if the character is alphanumeric, move ahead
		if ( isalpha(c) || isdigit(c) || c == '_' )
			_at_scanner_next_char(scanner);

		// non-alphanumeric, so break here
		else
			break;
	};

	// set the token
	out->string = start;
	out->length = scanner->p - start;
	out->line   = line;
	out->column = column;
	out->type   = at_lang_is_keyword(out->string, out->length) ? ATOM_TOKEN_KEYWORD : ATOM_TOKEN_IDENTIFIER;
	out->valid  = ATOM_TRUE;
}

/**
 * Scans the next number token
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_number(atScanner *scanner, atToken *out) {

	// cache the scanner state
	char *start         = scanner->p;
	unsigned int line   = scanner->line;
	unsigned int column = scanner->column;

	int chars_left    = _at_scanner_chars_left(scanner);
	atom_bool valid   = ATOM_TRUE;
	int decimal_count = 0;
	int exp_count     = 0;
	char c;


	while ( chars_left > 0 ) {
		c = _at_scanner_peek_char(scanner);
		--chars_left;

		// handle decimals
		if ( c == '.' ) {

			// make sure there is a maximum of one decimal
			if ( ++decimal_count == 2 ) {
				valid = ATOM_FALSE;
			}
		}
		// allow 'e' or 'E' for scientific notation
		else if ( c == 'e' || c == 'E' ) {
			if ( ++exp_count == 2 ) {
				valid = ATOM_FALSE;
			}
		}
		// make sure no other alphabetic characters are used
		else if ( isalpha(c) || c == '_' ) {
			valid = ATOM_FALSE;
			break;
		}
		// break if a non-numeric character is reached
		else if ( !isdigit(c) ) {
			break;
		}

		// move the scanner ahead
		_at_scanner_next_char(scanner);
	}

	// set the token
	out->string = start;
	out->length = scanner->p - start;
	out->line   = line;
	out->column = column;
	out->type   = ATOM_TOKEN_NUMBER;
	out->valid  = valid;
}

/**
 * Scans the next symbol token (includes '\n' and '\t')
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_symbol(atScanner *scanner, atToken *out) {

	char *p             = scanner->p;
	int len             = 1;
	unsigned chars_left = _at_scanner_chars_left(scanner);
	unsigned type       = ATOM_TOKEN_SEPARATOR;

	// check if the symbol is an operator
	switch ( *p ) {
		case '=': {
			type = ATOM_TOKEN_OPERATOR;
			if ( chars_left > 1 && *(p + 1) == '=' )
				len  = 2;
			break;
		}
		case '>': {
			type = ATOM_TOKEN_OPERATOR;
			if ( chars_left > 1 && *(p + 1) == '=')
				len = 2;
			break;
		}
		case '<': {
			type = ATOM_TOKEN_OPERATOR;
			if ( chars_left > 1 && *(p + 1) == '=')
				len = 2;
			break;
		}
		case '~': {
			type = ATOM_TOKEN_OPERATOR;
			if ( chars_left > 1 && *(p + 1) == '=')
				len = 2;
			break;
		}
		//case '\'':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case '.':
		case '!':


			type = ATOM_TOKEN_OPERATOR;
			break;
		default:
			break;
	}

	// set the token
	out->string = p;
	out->length = len;
	out->line   = scanner->line;
	out->column = scanner->column;
	out->type   = type;
	out->valid  = ATOM_TRUE;

	// move the scanner ahead
	for (int i = 0; i < len; ++i)
		_at_scanner_next_char(scanner);
}

/**
 * Scans the next string token inside of quotation marks
 * @param scanner The scanner
 * @param out     The output
 */
void _at_scanner_next_string(atScanner *scanner, atToken *out) {

	// move past the first quotation mark
	_at_scanner_next_char(scanner);

	// store the starting position of the string
	char *start = scanner->p;
	unsigned start_line = scanner->line;
	unsigned start_col  = scanner->column;
	int len = 0;
	char c;





	atom_bool escaped = ATOM_FALSE;

	while ( !at_scanner_is_eof(scanner) ) {
		// get the next character in the string
		c = _at_scanner_next_char(scanner);


		// check if the end of the string is reached
		if ( c == '"' || c == '\n' )
			break;


		++len;
	}

	// store the string
	out->type    = ATOM_TOKEN_STRING;
	out->string = start;
	out->length = len;
	out->line   = start_line;
	out->column = start_col;
	out->valid  = (c == '\"');
}


/**
 * Gets the next character in a scanner, incrementing the position and updating line/column counts
 * @param  scanner The scanner
 * @return         The next character
 */
char _at_scanner_next_char(atScanner *scanner) {

	if ( at_scanner_is_eof(scanner) )
		return 0;
	else {
		// get the next character
		char c = *scanner->p;
		++scanner->p;

		// increment line count
		if ( c == '\n' ) {
			++scanner->line;
			scanner->column = 1;
		}
		// increment the column count
		else
			++scanner->column;

		return c;
	}
}

/**
 * Gets the next character in a scanner without incrementing the position
 * @param  scanner The scanner
 * @return         The next character, or 0 if the scanner is at EOF
 */
char _at_scanner_peek_char(atScanner *scanner) {
	if ( at_scanner_is_eof(scanner) ) {
		return 0;
	}
	else {
		return *scanner->p;
	}
}

/**
 * Sets a token to EOF
 * @param token The token
 */
void _at_scanner_set_eof(atScanner *scanner, atToken *token) {
	token->string = NULL;
	token->length = 0;
	token->type   = ATOM_TOKEN_EOF;
	token->line   = scanner->line;
	token->column = scanner->column;
}


/**
 * Gets the number of characters left in the scanner (including the current one)
 * @param scanner The scanner
 */
unsigned int _at_scanner_chars_left(atScanner *scanner) {
	return scanner->length - (scanner->p - scanner->source);
}
