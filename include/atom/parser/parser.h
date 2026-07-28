/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: parser.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


 #ifndef ATOM_PARSER_PARSER_H
 #define ATOM_PARSER_PARSER_H


#include "atom/parser/scanner.h"


typedef struct _atParserFrame {
	atom_byte *bytecode;
	unsigned   bytecode_length;
} atParserFrame;

typedef struct _atParser {
	atScanner  scanner;
	atom_byte *bytecode;
	unsigned   bytecode_length;
	unsigned   bytecode_capacity;
	unsigned   error_count;

} atParser;


/**
 * Initializes a parser with a source code string
 * @param parser The parser
 * @param str    The string
 * @return       The parser
 */
atParser* at_parser_init(atParser *parser, const char *str);

/**
 * Frees resources associated with a parser.
 * @param parser The parser.
 */
void at_parser_free(atParser *parser);


/**
 * Runs a parser and produces bytecode
 * @param parser The parser
 */
void at_parser_parse(atParser *parser);









#endif
