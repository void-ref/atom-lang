/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: scanner.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_PARSER_SCANNER_H
#define ATOM_PARSER_SCANNER_H

#include "atom/types.h"
#include "atom/parser/token.h"




/**
 * A struct describing the state of a scanner
 */
typedef struct _atScanner {
	char        *source;
	char        *p;
	unsigned int length;
	unsigned int line;
	unsigned int column;
} atScanner;



/**
 * Initializes a scanner
 * @param  scanner       A pointer to the scanner to initialize
 * @param  source        The source string (this is copied)
 * @return               A pointer to 'scanner'
 */
atScanner* at_scanner_open(atScanner *scanner, const char *source);


/**
 * Frees resources used by the scanner
 * @param scanner A pointer to the scanner
 */
void at_scanner_close(atScanner *scanner);


/**
 * Checks if the scanner has reached the end
 * @param  scanner the scanner
 * @return         ATOM_TRUE if the scanner has reached EOF, ATOM_FALSE otherwise
 */
atom_bool at_scanner_is_eof(atScanner *scanner);


/**
 * Retrieves the next token in the scanner
 * @param  scanner A pointer to the scanner
 * @param  out     A pointer to token output
 * @return         The number of characters read or -1 if EOF is reached
 */
int at_scanner_next(atScanner *scanner, atToken *out);






#endif
