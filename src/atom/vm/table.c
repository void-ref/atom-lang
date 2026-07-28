/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/17/2018
 * @Filename: table.c
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#include "atom/vm/table.h"
#include "atom/vm/vm.h"
#include "atom/vm/value.h"

#include <stdlib.h>
#include <string.h>


/**
 * Initializes an empty table
 * @param  vm       The VM to allocate this table on
 * @param  table    The table to initialize
 * @return          The pointer passed in 'table'
 */
atTable* at_table_init(atVM *vm, atTable *table) {
	table->vm = vm;
	table->head = NULL;
	table->meta_table = NULL;

	return table;
}

/**
 * Frees data associated with the given table
 * @param table The table
 */
void at_table_free(atTable *table) {

	// remove all of the table entries
	atTableEntry *head = table->head;
	atTableEntry *next;
	while ( head != NULL ) {
		free(head->key);
		at_value_dereference(&head->value);

		// free the current entry (save a pointer to the next item)
		next = head->next;
		free(head);

		// move to the next item in the table
		head = next;
	}


	table->head = NULL;
	table->vm   = NULL;

	// at_table_free(table->meta_table);
}


/**
 * Duplicates a table (shallow copy)
 * @param dest   The copy destination
 * @param source The table to copy
 */
void at_table_copy(atTable *dest, atTable *source);


/**
 * Indexes a table and returns the value at the given index
 * @param table The table to index
 * @param  key  The key
 * @return      The value at 'key' or NULL if the entry does not exist
 */
atValue* at_table_get(atTable *table, const char *key) {

	// find the table entry with a matching key
	atTableEntry *head = table->head;
	while ( head != NULL && strcmp(head->key, key) != 0 )
		head = head->next;

	// return NULL if the key is not in the table
	if ( head == NULL )
		return NULL;
	else
		return &head->value;
}

/**
 * Sets the value at a given key in a table
 * @param table The table to index
 * @param key   The key
 * @param value The new value
 */
void at_table_set(atTable *table, const char *key, atValue *value) {

	atTableEntry *head = table->head;
	atTableEntry *last = NULL;
	while ( head != NULL ) {

		// the key alrady exists, so replace the current value
		if ( strcmp(head->key, key) == 0 ) {

			// dereference the old value
			at_value_dereference(&head->value);

			// check if the new value is null and the current entry needs to be deleted
			if ( value == NULL || value->type == ATOM_VALUE_NULL ) {

				// remove the entry from the linked list
				if ( last != NULL )
					last->next = head->next;
				else
					table->head = head->next;

				// free the entry key
				free(head->key);

				// free the table entry
				free(head);
			}
			else {
				// store the new value and reference it
				head->value = *value;
				at_value_reference(&head->value);
			}

			return;
		}
		// break early so we can hold onto the tail of the linked list
		else if ( head->next == NULL )
			break;
		// move to the next node
		else {
			last = head;
			head = head->next;
		}
	}

	// If we're here, then the key is not already in the table.

	// Storing nulls. Not even once.
	if ( value == NULL || value->type  == ATOM_VALUE_NULL ) {
		return;
	}


	// create a new table entry
	atTableEntry *entry = malloc(sizeof(atTableEntry));
	entry->next = NULL;

	// copy the key into the entry
	unsigned key_len = strlen(key);
	entry->key = malloc(key_len + 1);
	strcpy(entry->key, key);

	// store the value in the entry
	entry->value = *value;
	at_value_reference(&entry->value);


	// if 'head' is null, then there are no entries in the table yet
	if ( head == NULL )
		table->head = entry;
	// otherwise, add the entry to the tail end
	else
		head->next = entry;
}

/**
 * Stores a number in a table, overwriting the old value
 * @param table The table to index
 * @param key   The key
 * @param value The number
 */
void at_table_set_number(atTable *table, const char *key, atom_number num) {
	// create a value from the number
	atValue value;
	at_value_from_number(&value, num);

	// store the value in the table
	at_table_set(table, key, &value);
}


/**
 * Stores a user function in a table, overwriting the old value
 * @param table The table to index
 * @param key   The key
 * @param value The user function
 */
void at_table_set_cfunction(atTable *table, const char *key, atUserFunction func) {
	// create a value from the user function
	atValue value;
	at_value_from_cfunction(&value, func);

	// store the value in the table
	at_table_set(table, key, &value);
}
