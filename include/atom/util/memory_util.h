/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: memory_util.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#ifndef ATOM_UTIL_MEMORY_UTIL_H
#define ATOM_UTIL_MEMORY_UTIL_H




/**
 * Copies an array to a new one. YOU are responsible for
 * freeing both the new array and the old one.
 * @param array    The array to copy
 * @param size     The size of the array
 * @param new_size The size of the duplicate array
 * @return         The new array
 */
void* at_array_copy(void *array, unsigned size, unsigned new_size);



/**
 * Allocates memory for a string, then copies it
 * @param  str The string
 * @return     The copy
 */
char* at_string_dynamic_copy(const char *str);




#endif
