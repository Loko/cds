/**
 * @file cds_dynamic_array.h
 * @author Jeff Lansing
 * A dynamic array, growable in size, similar to stl's vector in C++.
 * Default growth rate of 1.5 when filled.  Supports variable removal 
 * behavoirs and growth rates. Optimized for insertions and removals 
 * from the end of the container thru push_back and pop_back.
 */
 
#ifndef _CDS_DYNAMIC_ARRAY_H_
#define _CDS_DYNAMIC_ARRAY_H_
 
/** 
 * @addtogroup CDS_DYNAMIC_ARRAY
 * @{
 */
  
#include "cds_core.h"

/** Default reccomended starting size */
#define CDS_DEFAULT_DYNAMIC_ARRAY_SIZE 4

/** 
 * Default growth rate when an existing dynamic array's capacity is 
 * reached.  This can be avoided entirely in favor of your own growth 
 * function.
 */
#define CDS_DEFAULT_DYNAMIC_ARRAY_GROWTH_MULTIPLIER 1.5

/** 
 * Lets the user specify how they want the size of the dynamic array to 
 * grow/shrink.  By default it will grow by 1.5 each time the capacity is 
 * reached.  The callback takes the existing size as an argument and returns 
 * the newly desired size of the array if the passed in call back is NULL, 
 * the default will be used.
 */
typedef unsigned int (*cds_growth_func)(unsigned int);

/**
 * How might we handle a cleared array?
 */
typedef unsigned int (*cds_resize_func)(unsigned int, unsigned int);

/** 
 * Define the ways to handle removal from the middle.
 * e.g. shift down, or replace with last element
 */
enum cds_removal_behavoir {
	CDS_SHIFT_DOWN = 0,
	CDS_REPLACE_WITH_LAST = 1
};

/** @typedef cds_removal_behavoir */
typedef enum cds_removal_behavoir cds_removal_behavoir;

/** The default removal behavoir */
#define CDS_DEFAULT_REMOVAL_BEHAVOIR CDS_SHIFT_DOWN

/** 
 * General purpose structure for a dynamically allocated and growable array.
 * Uses void ** pointers to allow usage with any type, including other arrays 
 * (e.g. char *). Count is the amount of elements, size is the capacity of the
 * container itself.
 * @todo support sorting routines
 */
struct cds_dynamic_array {
	void **buffer;
	unsigned int count;
	unsigned int size;
};

/** @typedef cds_dynamic_array type */
typedef struct cds_dynamic_array cds_dynamic_array;

/** 
 * Creates the dynamic array with the given starting size. 
 * @param size A size of zero is valid, but the buffer will not be allocated
 */
cds_result cds_dynamic_array_create(cds_dynamic_array **array, unsigned int size);

/** Resizes the capacity of the array */
cds_result cds_dynamic_array_resize(cds_dynamic_array **array, unsigned int size);

/** Deletes the array */
cds_result cds_dynamic_array_delete(cds_dynamic_array **array);

/** Deletes the array and the data pointers */
cds_result cds_dynamic_array_delete_all(cds_dynamic_array **array);

/** Removes every element from the container */
cds_result cds_dynamic_array_clear(cds_dynamic_array *array);

/** Gets the count of the array */
unsigned int cds_dynamic_array_count(const cds_dynamic_array *array);

/** Gets the size (capacity) of the array */
unsigned int cds_dynamic_array_size(const cds_dynamic_array *array);

/** Puts the element at the given index into data */
cds_result cds_dynamic_array_get(const cds_dynamic_array *array, unsigned int index, void **data);

/** Sets the value of the array at the given index */
cds_result cds_dynamic_array_set(cds_dynamic_array *array, unsigned int index, const void *data);

/** Adds the data to the end of the container */
cds_result cds_dynamic_array_push_back(cds_dynamic_array *array, const void *data);

/** Adds the data to the end of the container and grows, if necessary, using the growth function */
cds_result cds_dynamic_array_push_back_gf(cds_dynamic_array *array, const void *data, cds_growth_func growth_func);

/** Removes the last element from the array */
cds_result cds_dynamic_array_pop_back(cds_dynamic_array *array);

/** Removes the last element from the array, puts it into data */
cds_result cds_dynamic_array_pop_back_data(cds_dynamic_array *array, void **data);

/** Removes the element with the given address */
cds_result cds_dynamic_array_remove(cds_dynamic_array *array, const void *data);

/** Removes the element with the given address using the given removal behavoir */
cds_result cds_dynamic_array_remove_rb(cds_dynamic_array *array, const void *data, cds_removal_behavoir rb);

/** Removes the element using the given comparison function */
cds_result cds_dynamic_array_remove_cmp(cds_dynamic_array *array, const void *data, cds_cmp_func cmp_func);

/** Removes the element using the given comparison function using the given removal behavoir */
cds_result cds_dynamic_array_remove_cmp_rb(cds_dynamic_array *array, const void *data, cds_cmp_func cmp_func, cds_removal_behavoir rb);

/** Removes the element from the array at the given index */
cds_result cds_dynamic_array_remove_at(cds_dynamic_array *array, unsigned int index);

/** Removes the element from the array at the given index using the given removal behavoir */
cds_result cds_dynamic_array_remove_at_rb(cds_dynamic_array *array, unsigned int index, cds_removal_behavoir rb);

/** Searches for the address, puts the index into the index pointer */
cds_result cds_dynamic_array_find(const cds_dynamic_array *array, const void *data, unsigned int *index);

/** Searches for the element using the comparison function, puts the index into the index pointer */
cds_result cds_dynamic_array_find_cmp(const cds_dynamic_array *array, const void *data, unsigned int *index, cds_cmp_func cmp_func);

/** Reverses the order of elements in the array */
cds_result cds_dynamic_array_reverse(cds_dynamic_array *array);

/** Iterates over every element in the container */
cds_result cds_dynamic_array_iterate(const cds_dynamic_array *array, cds_visit_func visit_func);

/** @} */

#endif
