#ifndef _CDS_CORE_H_
#define _CDS_CORE_H_

#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"

/* LOGGING */
// logging support is not provided, but easy to plug into cds 
// with a simple function pointer with a printf format
/*
typedef int (*cds_log_func)(const char*, ...);

// functions to get/set cds_log
int cds_log(const char *format, ...);
*/

/* ASSERTIONS */
//										   		   // condition   // file	 // line // message // va args
typedef void (*cds_assertion_failure_reporter_func)(const char*, const char*, int, const char*, va_list); 

// the default assertion reporting handler
void cds_default_assertion_failure_reporter(const char *condition, const char *file, int line, const char *message, va_list argp);

// mutable assertion reporter function used in the assertion macro
cds_assertion_failure_reporter_func cds_assertion_reporter;

// called by the macro if the assertion fails
// if an assertion handler is assigned, it is invoked
// regardless, afterwards the program is exited
void cds_on_assertion_failure(const char *condition, const char *file, int line, const char *message, ...);

//#define CDS_ASSERTIONS_ENABLED

#define CDS_HALT() asm { int 3 }
#define CDS_UNUSED(expr) do { (void)sizeof(expr); } while(0)

#ifdef CDS_ASSERTIONS_ENABLED
	#define CDS_ASSERT(expr) \
		do \
		{ \
			if (!(expr)) \
			{ \
				cds_on_assertion_failure(#expr, \
										 __FILE__, \
										 __LINE__, \
										 (NULL)); \
			} \
		} while (0)
	#define CDS_ASSERT_MSG(expr, msg, ...) \
		do \
		{ \
			if (!(expr)) \
			{ \
				cds_on_assertion_failure(#expr, \
										 __FILE__, \
										 __LINE__, \
										 (msg), \
										 __VA_ARGS__); \
			} \
		} while (0)
#else
	#define CDS_ASSERT(expr) \
		do { CDS_UNUSED(expr); } while(0)
	#define CDS_ASSERT_MSG(expr, msg, ...) \
		do { CDS_UNUSED(expr); CDS_UNUSED(msg); } while(0)
#endif


/* ERROR CODES */

// enum for return values describing the success/failure of a function
// cds_result should probably be capitalized
// Had an idea the other day that this enum is a bunch of bitfields and ORing them could reveal multiple errors
// But this may be difficult to implement, and would conflict with the idea of early returns?
enum cds_result {
	CDS_UNIMPLEMENTED = -10,
	CDS_DUPLICATE_VALUE = -9,
	CDS_BAD_ALLOC = -8,
	CDS_OVERFLOW = -7,
	CDS_UNDERFLOW = -6,
	CDS_INDEX_OUT_OF_RANGE = -5,
	CDS_NULL_ARGUMENT = -4,
	CDS_INVALID_OPERATION = -3,
	CDS_INVALID_ARGUMENT = -2,
	CDS_NOT_FOUND = -1,
	CDS_OK = 1
};

typedef enum cds_result cds_result;

// determines if the result indicates a failed routine
// everything except CDS_OK and CDS_NOT_FOUND is an illegal 
// operation/error that will likely require code changes
int cds_is_error(cds_result r);

#define CDS_MAX_ERR_STR_LEN 50

// returns a human readable string for the result
// returns the size of the string, or a negative number if the operation failed
int cds_result_string(cds_result r, char *str);

// 
int cds_error_check(cds_result r);


/* MEMORY MANAGEMENT */
// function pointers with the same format as those in the C std lib
typedef void* (*cds_alloc_func)(size_t);
typedef void* (*cds_realloc_func)(void *, size_t);
typedef void  (*cds_free_func)(void *);

// passing NULL will use the default allocator, reallocator, and free functions
void cds_set_alloc(cds_alloc_func alloc_func);
void cds_set_realloc(cds_realloc_func realloc_func);
void cds_set_free(cds_free_func free_func);

// can use accessors to compare to the function address you wish to use
// these should never return NULL
cds_alloc_func cds_get_alloc();
cds_realloc_func cds_get_realloc();
cds_free_func cds_get_free();

void * cds_alloc(size_t size);
void * cds_realloc(void *ptr, size_t size);
void cds_free(void *ptr);

/*
cds_alloc_func cds_allocator;
cds_realloc_func cds_reallocator;
cds_free_func cds_freer;
*/

/* MISCELLANOUS */
typedef int (*cds_cmp_func)(void *, void *);
typedef void (*cds_visit_func)(void *);
typedef unsigned int (*cds_hash_func)(void *);
#endif