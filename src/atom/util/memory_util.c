/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: memory_util.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#include "atom/util/memory_util.h"

#include <string.h>
#include <stdlib.h>
#include <memory.h>


/**
 * Copies an array to a new one
 * @param array    The array to copy
 * @param size     The size of the array
 * @param new_size The size of the duplicate array
 * @return         The new array
 */
void* at_array_copy(void *array, unsigned size, unsigned new_size) {

	// allocate a new array
	void *new_array = malloc(new_size);
	memset(new_array, 0, new_size);

	// The length to copy is whichever size is lesser
	int copy_size = size > new_size ? new_size : size;

	// copy the array
	if ( array != NULL && copy_size != 0 )
		memcpy(new_array, array, copy_size);

	return new_array;
}


/**
 * Allocates memory for a string, then copies it
 * @param  str The string
 * @return     The copy
 */
char* at_string_dynamic_copy(const char *str) {
	unsigned length = strlen(str);
	char *copy = malloc(length + 1);
	strncpy(copy, str, length + 1);
	return copy;
}
