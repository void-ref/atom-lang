/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: table.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */

#ifndef ATOM_VM_TABLE_H
#define ATOM_VM_TABLE_H


#include "atom/vm/value.h"

typedef struct _atVM atVM;


typedef struct _atTableEntry {
	char   *key;
	atValue value;

	struct _atTableEntry *next;
} atTableEntry;

/**
 * Pairs a string key with an atValue
 */
typedef struct _atTable {
	atTableEntry *head;
	atVM     *vm;
	struct _atTable *meta_table;
} atTable;


/**
 * Initializes an empty table
 * @param  vm       The VM to allocate this table on
 * @param  table    The table to initialize
 * @return          The pointer passed in 'table'
 */
atTable* at_table_init(atVM *vm, atTable *table);

/**
 * Frees data associated with the given table
 * @param vm    The VM the table was allocated on
 * @param table [description]
 */
void at_table_free(atTable *table);


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
atValue* at_table_get(atTable *table, const char *key);

/**
 * Sets the value at a given key in a table
 * @param table The table to index
 * @param key   The key
 * @param value The new value
 */
void at_table_set(atTable *table, const char *key, atValue *value);


/**
 * Stores a number in a table, overwriting the old value
 * @param table The table to index
 * @param key   The key
 * @param value The number
 */
void at_table_set_number(atTable *table, const char *key, atom_number value);

/**
 * Stores a user function in a table, overwriting the old value
 * @param table The table to index
 * @param key   The key
 * @param value The user function
 */
void at_table_set_cfunction(atTable *table, const char *key, atUserFunction value);






#endif
