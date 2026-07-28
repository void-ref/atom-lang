/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: lang.h
 * @Last modified by:   mitchell
 * @Last modified time: 11/30/2018
 */



#ifndef ATOM_PARSER_LANG_H
#define ATOM_PARSER_LANG_H

#include "atom/types.h"

#define ATOM_LANG_COMMENT_CHAR '#'


/**
 * Checks if a given string is a keyword
 * @param  word   The string
 * @param  length The length of the string. Use 0 for null-terminated strings
 * @return        ATOM_TRUE if it is a keyword, ATOM_FALSE if not
 */
atom_bool at_lang_is_keyword(const char *str, int length);

/**
 * Returns the ID of a keyword
 * @param  str The keyword string
 * @param  length The length of the string. Use 0 for null-terminated strings
 * @return     The keyword ID, or -1 if it is not a keyword
 */
int at_lang_get_keyword_id(const char *str, int length);



#endif
