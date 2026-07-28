/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/18/2018
 * @Filename: token.c
 * @Last modified by:   mitchell
 * @Last modified time: 11/30/2018
 */

#include "atom/parser/token.h"

#include <stdio.h>



/**
 * Returns the id of an operator token
 * @param  token The token
 * @return       The id
 */
unsigned at_token_operator_type(atToken *token) {

	// make sure this is an operator token
	if ( token->type != ATOM_TOKEN_OPERATOR ) {
		return ATOM_OPERATOR_INVALID;
	}

	if ( token->length == 1 ) {
		switch ( *token->string ) {
			case '+':
				return ATOM_OPERATOR_ADD;
			case '-':
				return ATOM_OPERATOR_SUB;
			case '*':
				return ATOM_OPERATOR_MUL;
			case '/':
				return ATOM_OPERATOR_DIV;
			case '%':
				return ATOM_OPERATOR_MOD;
			case '^':
				return ATOM_OPERATOR_EXP;
			case '!':
				return ATOM_OPERATOR_FACT;

			case '=':
				return ATOM_OPERATOR_ASSIGN;

			case '>':
				return ATOM_OPERATOR_GT;
			case '<':
				return ATOM_OPERATOR_LT;
			case '~':
				return ATOM_OPERATOR_NOT;


			case '.':
				return ATOM_OPERATOR_INDEX;

			default:
				return ATOM_OPERATOR_INVALID;
		}
	}
	else {
		if ( token->string[0] == '=' && token->string[1] == '=' ) {
			return ATOM_OPERATOR_EQS;
		}
		else if ( token->string[0] == '~' && token->string[1] == '=' ) {
			return ATOM_OPERATOR_NEQS;
		}
		else if ( token->string[0] == '<' && token->string[1] == '=' ) {
			return ATOM_OPERATOR_LTE;
		}
		else if ( token->string[0] == '>' && token->string[1] == '=' ) {
			return ATOM_OPERATOR_GTE;
		}
	}

	return ATOM_OPERATOR_INVALID;

}


/**
 * Prints a token using printf
 * @param token The token to print
 */
void at_token_print(atToken *token) {


	printf("[");

	// print the token type
	switch ( token->type ) {
		case ATOM_TOKEN_EOF:
			printf("EOF,        ");
			break;
		case ATOM_TOKEN_IDENTIFIER:
			printf("identifier, ");
			break;
		case ATOM_TOKEN_KEYWORD:
			printf("keyword,    ");
			break;
		case ATOM_TOKEN_NUMBER:
			printf("number,     ");
			break;
		case ATOM_TOKEN_STRING:
			printf("string,     ");
			break;
		case ATOM_TOKEN_SEPARATOR:
			printf("separator,  ");
			break;
		case ATOM_TOKEN_OPERATOR:
			printf("operator,   ");
			break;
		default:
			printf("invalid,    ");
			break;
	}

	if ( token->valid )
		printf("valid,   ");
	else
		printf("invalid, ");

	// print the token location
	printf("%4u:%4u]: ", token->line, token->column);

	// print special values for separator tokens (newline and tab)
	if ( token->type == ATOM_TOKEN_SEPARATOR ) {

		// print char-by-char in case there is special character
		for (int i = 0; i < token->length; ++i) {
			switch ( token->string[i] ) {
				case '\t':
					printf("\\t");
					break;
				case '\n':
					printf("\\n");
					break;
				default:
					printf("%c", token->string[i]);
			}
		}
	}
	// default print
	else
		printf("%*.*s", token->length, token->length, token->string);

	printf("\n");
}
