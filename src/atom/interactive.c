/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: interactive.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#include "atom/interactive.h"
#include "atom/atom.h"


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


void prompt_for_code(char **output);



/**
 * Runs Atom interactive mode on the command line
 */
void at_run_interactive_mode(atVM *vm) {

	// Create a virtual machine
	at_vm_init(vm);




	printf("Running atom interactive mode. Type 'exit' to quit.\n");



	char *code = NULL;

	// continually prompt user for code
	while ( 1 ) {
		printf(">> ");

		// free the last string entered
		free(code);

		// read a new string from the user
		prompt_for_code(&code);

		// exit if requested
		if ( code == NULL || strcmp(code, "exit") == 0 ) {
			printf("\n");
			break;
		}

		// execute code
		at_executor_run_source(&vm->executor, code);

		atValue *top = at_vm_top(vm);
		if ( !(top == NULL || top->type == ATOM_VALUE_NULL) ) {
			at_value_print(top);
			at_vm_pop(vm);
		}
	}




}



void prompt_for_code(char **output) {

	unsigned buffer_capacity = 128;
	char *buffer = malloc(buffer_capacity);
	int c;
	int i = 0;

	// read characters until a new line is reached
	while ( (c = getchar()) != EOF && c != '\n' ) {

		// if the buffer capacity is reached, resize it
		if ( i == buffer_capacity ) {
			unsigned new_capacity = buffer_capacity * 2;

			char *temp = malloc(new_capacity);
			memcpy(temp, buffer, buffer_capacity);
			free(buffer);
			buffer = temp;
		}

		// store the character read in the buffer
		buffer[i] = c;
		++i;
	}

	// EOF signals termination
	if ( c == EOF ) {
		free(buffer);
		*output = NULL;
		return;
	}

	// terminate the string
	buffer[i] = '\0';

	// set the output buffer
	*output = buffer;
}
