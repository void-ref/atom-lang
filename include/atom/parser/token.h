/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/18/2018
 * @Filename: token.h
 * @Last modified by:   mitchell
 * @Last modified time: 11/30/2018
 */


#ifndef ATOM_PARSER_TOKEN_H
#define ATOM_PARSER_TOKEN_H

#include "atom/types.h"



#define ATOM_TOKEN_INVALID     0
#define ATOM_TOKEN_EOF         1
#define ATOM_TOKEN_IDENTIFIER  2
#define ATOM_TOKEN_KEYWORD     3
#define ATOM_TOKEN_NUMBER      4
#define ATOM_TOKEN_STRING      5
#define ATOM_TOKEN_SEPARATOR   6 // For things like parenthesis, colons, newlines, tabs, etc.
#define ATOM_TOKEN_OPERATOR    7


typedef enum _atOperator {
	ATOM_OPERATOR_INVALID,

	ATOM_OPERATOR_ASSIGN,

	ATOM_OPERATOR_ADD,
	ATOM_OPERATOR_SUB,
	ATOM_OPERATOR_MUL,
	ATOM_OPERATOR_DIV,
	ATOM_OPERATOR_MOD,
	ATOM_OPERATOR_EXP,
	ATOM_OPERATOR_FACT,

	ATOM_OPERATOR_EQS,
	ATOM_OPERATOR_NEQS,
	ATOM_OPERATOR_NOT,
	ATOM_OPERATOR_GT,
	ATOM_OPERATOR_GTE,
	ATOM_OPERATOR_LT,
	ATOM_OPERATOR_LTE,


	ATOM_OPERATOR_INDEX,
} atOperator;



/**
 * A struct describing a token
 */
typedef struct _atToken {
	const char  *string;
	unsigned int length;
	unsigned int type;
	unsigned int column;
	unsigned int line;
	atom_bool    valid;
} atToken;









/**
 * Prints a token using printf
 * @param token The token to print
 */
void at_token_print(atToken *token);



/**
 * Returns the id of an operator token
 * @param  token The token
 * @return       The id
 */
unsigned at_token_operator_type(atToken *token);




#endif
