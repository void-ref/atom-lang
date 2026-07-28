/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: lang.c
 * @Last modified by:   mitchell
 * @Last modified time: 12/02/2018
 */

#include "atom/parser/lang.h"

#include <string.h>

const char* _at_keyword_strings[] = {
	"if",
	"else",
	"func",
	"end",
	"null",
	"and",
	"or",
	"xor",
	"sdjoigjeaoigjoieagiajgoia",
};
#define ATOM_NUM_KEYWORDS (sizeof(_at_keyword_strings) / sizeof(const char*))


/**
 * Checks if a given string is a keyword
 * @param  word   The string
 * @param  length The length of the string. Use 0 for null-terminated strings
 * @return        ATOM_TRUE if it is a keyword, ATOM_FALSE if not
 */
atom_bool at_lang_is_keyword(const char *str, int length) {
	return at_lang_get_keyword_id(str, length) != -1;
}

/**
 * Returns the ID of a keyword
 * @param  str The keyword string
 * @param  length The length of the string. Use 0 for null-terminated strings
 * @return     The keyword ID, or -1 if it is not a keyword
 */
int at_lang_get_keyword_id(const char *str, int length) {
	atom_bool match;

	// search the keyword list
	for (int i = 0; i < ATOM_NUM_KEYWORDS; ++i) {

		// make sure the lengths match
		if ( length != strlen(_at_keyword_strings[i]) ) {
			continue;
		}
		// check if the strings match
		else if ( strncmp(str, _at_keyword_strings[i], length) == 0 ) {
			return i;
		}
	}

	return -1;
}
