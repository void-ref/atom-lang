/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: file_util.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */


#ifndef ATOM_UTIL_FILE_UTIL_H
#define ATOM_UTIL_FILE_UTIL_H

#include "atom/types.h"


/**
 * Reads a text file into a string
 * @param  file The file to read
 * @param  out  A pointer to a string. The new string is malloc'ed, so YOU must free it!
 * @return      ATOM_TRUE if successful, ATOM_FALSE otherwise
 */
atom_bool at_read_text_file(const char *file, char **out);



#endif
