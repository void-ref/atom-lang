/**
 * @Author: Mitchell Kelly <mtmk>
 * @Date:   11/16/2018
 * @Filename: types.h
 * @Last modified by:   mtmk
 * @Last modified time: 11/29/2018
 */



#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define ATOM_TRUE  1
#define ATOM_FALSE 0


typedef uint8_t   atom_byte;
typedef int8_t    atom_bool;
typedef int16_t   atom_short;
typedef int32_t   atom_int;
typedef int64_t   atom_long;
typedef int16_t   atom_ushort;
typedef uint32_t  atom_uint;
typedef uint64_t  atom_ulong;
typedef float     atom_float;
typedef double    atom_double;
typedef void*     atom_pointer;

typedef double atom_number;







// typedefs for VM structs
typedef struct _atVM        atVM;
typedef struct _atExecutor  atExecutor;
typedef struct _atTable     atTable;
typedef struct _atFunction  atFunction;
typedef struct _atValue     atValue;

// typedefs for parser structs
typedef struct _atParser atParser;


typedef unsigned (*atUserFunction)(atVM *vm, unsigned arg_count);





#endif
