/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: cmd_tool.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/28/2018
 *
 * Contains the main function for running this tool
 * on the command line
 */

#include "atom/atom.h"
#include "atom/vm/vm.h"
#include "atom/interactive.h"
#include "atom/parser/scanner.h"
#include "atom/util/file_util.h"


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, const char* argv[]) {

	// Print the usage if no arguments are supplied
	if ( argc == 1 ) {
		printf("Usage: atm [-d] [-s] file\t- Executes a script file\n");
		printf("       atm [-d] -i       \t- Runs interactive mode\n\n");
		printf("       Use -d to enable debug messages.\n");

		return 0 ;
	}


	// Get the current working directory
	char cwd[128];
	if ( getcwd(cwd, 128) == NULL ) {
		printf("Error: cwd is not valid\n.");
		return 1;
	}

	atVM vm;
	vm.debug = ATOM_FALSE;
	at_vm_init(&vm);

	// Handle arguments
	for (int i = 1; i < argc; ++i) {

		// Handle bytecode files
		if ( strcmp(argv[i], "-b") == 0 ) {
			if ( i + 1 >= argc ) {
				printf("Error: no input files provided.\n");
				return 1;
			}

			const char *bytecode_file = argv[i + 1];
			printf("Executing bytecode file '%s'\n", bytecode_file);

			printf("Not implemented yet.\n");
			return 1;
		}
		// Handle source files
		else if ( strcmp(argv[i], "-s") == 0 ) {
			if ( i + 1 >= argc ) {
				printf("Error: no input files provided.\n");
				return 1;
			}
			printf("%s\n", argv[i]);

			const char *source_file = argv[i + 1];
			printf("Compiling source file '%s'\n", source_file);
			return 1;
		}
		else if ( strcmp(argv[i], "-i") == 0 ) {
			at_run_interactive_mode(&vm);
			break;
		}
		else if ( strcmp(argv[i], "-d") == 0 ) {
			vm.debug = ATOM_TRUE;
		}
		// Handle source files
		else if ( i == 1 ) {
			char *source;
			if ( !at_read_text_file(argv[i], &source) ) {
				printf("Could not open file '%s'.\n", argv[i]);
				return 1;
			}


			at_executor_run_source(&vm.executor, source);


			break;
		}
	}

	at_vm_terminate(&vm);

	return 0;
}
