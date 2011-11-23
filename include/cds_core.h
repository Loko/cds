/**
 * @file cds_core.h
 * @author Jeff Lansing
 * @brief A Collection of Base Functions for dynamic memory
 * allocations, assertions, error handling, logging, etc.
 */

#ifndef _CDS_CORE_H_
#define _CDS_CORE_H_

/** 
 * @addtogroup CDS
 * @{
 */

/** @todo preprocessor macro not to use the standard library?? */
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"

/* LOGGING */

/** Function prototype for printf-like logging functions using va_lists */
typedef int (*cds_log_func)(const char*, va_list);

/** Gets the custom log function used by @see cds_log */
cds_log_func cds_get_log();

/** Sets the custom log function which will be used by cds_log */
void cds_set_log(cds_log_func lf);

/** The main logging function used by cds */
int cds_log(const char *format, ...);


/* ASSERTIONS */

/** @typedef cds_assertion_failure_reporter_func */
typedef void (*cds_assertion_failure_reporter_func)(const char*, const char*, int, const char*, va_list); 

/** Gets the assertion reporting function */
cds_assertion_failure_reporter_func cds_get_assertion_reporter();

/** Sets the assertion reporting function */
void cds_set_assertion_reporter(cds_assertion_failure_reporter_func reporter_func);

/** 
 * Called by the macro if the assertion fails.
 * if an assertion handler is assigned, it is invoked
 * regardless, afterwards the program is exited
 */
void cds_on_assertion_failure(const char *condition, const char *file, int line, const char *message, ...);

#define CDS_ASSERTIONS_ENABLED

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


/* ERROR/STATUS CODES */

/** 
 * Enum for return values describing the success/failure of a function.
 * @todo Had an idea the other day that this enum is a bunch of bitfields 
 * and ORing them could reveal multiple errors.  But this may be difficult 
 * to implement, and would conflict with the idea of early returns?  Also, 
 * consider using the FMOD standards, where ERR is put in the name
 */
enum cds_result {
	CDS_UNIMPLEMENTED = -10,    /**< A feature that's not yet supported */
	CDS_DUPLICATE_VALUE = -9,   /**< A duplicate insertion into a set like structure */
	CDS_BAD_ALLOC = -8,         /**< A failed memory allocation */
	CDS_OVERFLOW = -7,          /**< An overflow of insertions into a container */
	CDS_UNDERFLOW = -6,         /**< An underflow of removals from a container */
	CDS_INDEX_OUT_OF_RANGE = -5,/**< An index outside the bounds of a container */
	CDS_NULL_ARGUMENT = -4,     /**< An invalid NULL pointer argument */
	CDS_INVALID_OPERATION = -3, /**< A generic return for improper operations */
	CDS_INVALID_ARGUMENT = -2,  /**< A generic return for imporper arguments passed to a function */
	CDS_NOT_FOUND = -1,         /**< Indicates the search value was not found in the container (not a runtime error) */
	CDS_OK = 1                  /**< Function completed successfully */
};

/** @typedef Custom yype for the enum of the same name */
typedef enum cds_result cds_result;

/**
 * Determines if the result indicates a failed routine.
 * Everything except CDS_OK, CDS_NOT_FOUND, and CDS_DUPLICATE_VALUE 
 * is an illegal operation/runtime error that will likely require 
 * bug fixes through code changes.
 */
int cds_is_error(cds_result cr);

/** Maximum size of a cds_result string */
#define CDS_RESULT_MAX_STR_LEN 50

/**
 * Puts a human readable string for the result into str
 * @return The size of the string, or -1 if the operation fails
 */
int cds_result_string(cds_result cr, char *str);

/** Will print the result and return 1 if it is an error */
int cds_error_check(cds_result cr);

/** Will print the result and return 1 if cr != CDS_OK */
int cds_not_ok_check(cds_result cr);

/* MEMORY MANAGEMENT */

/** A prototype for allocators.  Should return NULL on a failed allocation. */
typedef void* (*cds_alloc_func)(size_t);
/** A prototype for reallocators.  Should return NULL on a failed reallocation. */
typedef void* (*cds_realloc_func)(void *, size_t);
/** A prototype for free/delete functions.  */
typedef void  (*cds_free_func)(void *);

/** Sets the allocator */
void cds_set_alloc(cds_alloc_func alloc_func);
/** Sets the reallocator */
void cds_set_realloc(cds_realloc_func realloc_func);
/** Sets the free func */
void cds_set_free(cds_free_func free_func);

/** Gets the allocator */
cds_alloc_func cds_get_alloc();
/** Gets the reallocator */
cds_realloc_func cds_get_realloc();
/** Gets the free func */
cds_free_func cds_get_free();

/** Allocation call used by the CDS libraries */
void * cds_alloc(size_t size);
/** Reallocation call used by the CDS libraries */
void * cds_realloc(void *ptr, size_t size);
/** Free call used by the CDS libraries */
void cds_free(void *ptr);

/* MISCELLANOUS FUNCTION PROTOTYPES */

/** Prototype for a compare by value function that takes any type */
typedef int (*cds_cmp_func)(const void *, const void *);
/** Prototype for a visiting function (used by iterators) */
typedef void (*cds_visit_func)(const void *);
/** Prototype for a visiting pair function (use by iterators) */
typedef void (*cds_visit_pair_func)(const void *, const void *);
/** Prototype for a hash function of any type */
typedef unsigned int (*cds_hash_func)(const void *);

/** @} */
#endif
