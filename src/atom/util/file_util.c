/**
 * File:   file_util.c
 * Author: Mitchell Matsumori-Kelly
 * Date:   11/16/2018
 */

#include "atom/util/file_util.h"

#include <stdio.h>
#include <stdlib.h>




/**
 * Reads a text file into a string
 * @param  file The file to read
 * @param  out  A pointer to a string. The new string is malloc'ed, so YOU must free it!
 * @return      ATOM_TRUE if successful, ATOM_FALSE otherwise
 */
atom_bool at_read_text_file(const char *file, char **out) {

	// try to open the file
	FILE *in = fopen(file, "rb");
	if ( !in )
		return ATOM_FALSE;

	// get the size of the file in bytes (chars, essentially)
	fseek(in, 0L, SEEK_END);
	unsigned long size = ftell(in);
	rewind(in);

	// create a buffer for string
	char *buffer = malloc(size + 1);
	buffer[size] = '\0';

	// copy the file into the buffer
	if ( fread(buffer, size, 1, in) != 1 ) {
		fclose(in);
		free(buffer);
		return ATOM_FALSE;
	}

	// close the file
	fclose(in);

	// set the output buffer
	*out = buffer;

	return ATOM_TRUE;
}
