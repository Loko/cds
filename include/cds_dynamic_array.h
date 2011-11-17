/*
 * A dynamic array similar in functionality to the vector container in C++
 * Default growth rate of 1.5 when filled.  Supports variable removal behavoirs and growth rates.
 * Optimized for insertions and removals from the end of the container thru push_back and pop_back
 */
#ifndef _CDS_DYNAMIC_ARRAY_H_
#define _CDS_DYNAMIC_ARRAY_H_
#include "cds_core.h"

// default reccomended size
#define CDS_DEFAULT_DYNAMIC_ARRAY_SIZE 4

// default growth rate when an existing dynamic array's capacity is reached
// this can be avoided entirely in favor of your own growth function
#define CDS_DEFAULT_DYNAMIC_ARRAY_GROWTH_MULTIPLIER 1.5

// lets the user specify how they want the size of the dynamic array to grow
// by default it will grow by 1.5 each time the capacity is reached
// the callback takes the existing size as an argument and returns the newly desired size of the array
// if the passed in call back is NULL, the default will be used
typedef unsigned int (*cds_growth_func)(unsigned int);

// define the ways to handle removal from the middle 
// e.g. shift down, or replace with last element
enum cds_removal_behavoir {
	CDS_SHIFT_DOWN = 0,
	CDS_REPLACE_WITH_LAST = 1
};
typedef enum cds_removal_behavoir cds_removal_behavoir;

// the default removal behavoir
#define CDS_DEFAULT_REMOVAL_BEHAVOIR CDS_SHIFT_DOWN

// general purpose structure for a dynamically allocated and growing array
// uses void ** pointers to allow usage with any type, including other arrays (e.g. char *)
// count is the amount of elements
// size is the allocated size, in elements not bytes
struct cds_dynamic_array {
	void **buffer;
	unsigned int count;
	unsigned int size;
};
typedef struct cds_dynamic_array cds_dynamic_array;

cds_result cds_dynamic_array_create(cds_dynamic_array **array, const unsigned int size);
cds_result cds_dynamic_array_resize(cds_dynamic_array **array, const unsigned int size);
cds_result cds_dynamic_array_delete(cds_dynamic_array **array);
cds_result cds_dynamic_array_delete_all(cds_dynamic_array **array);
cds_result cds_dynamic_array_clear(cds_dynamic_array *array);

unsigned int cds_dynamic_array_count(const cds_dynamic_array *array);
unsigned int cds_dynamic_array_size(const cds_dynamic_array *array);
cds_result cds_dynamic_array_get(const cds_dynamic_array *array, const unsigned int index, void **data);
cds_result cds_dynamic_array_set(cds_dynamic_array *array, const unsigned int index, const void *data);

cds_result cds_dynamic_array_push_back(cds_dynamic_array *array, const void *data);
cds_result cds_dynamic_array_push_back_gf(cds_dynamic_array *array, const void *data, const cds_growth_func growth_func);
cds_result cds_dynamic_array_pop_back(cds_dynamic_array *array);
cds_result cds_dynamic_array_pop_back_data(cds_dynamic_array *array, void **data);

cds_result cds_dynamic_array_remove(cds_dynamic_array *array, const void *data);
cds_result cds_dynamic_array_remove_rb(cds_dynamic_array *array, const void *data, const cds_removal_behavoir rb);
cds_result cds_dynamic_array_remove_cmp(cds_dynamic_array *array, const void *data, const cds_cmp_func cmp_func);
cds_result cds_dynamic_array_remove_cmp_rb(cds_dynamic_array *array, const void *data, const cds_cmp_func cmp_func, const cds_removal_behavoir rb);
cds_result cds_dynamic_array_remove_at(cds_dynamic_array *array, const unsigned int index);
cds_result cds_dynamic_array_remove_at_rb(cds_dynamic_array *array, const unsigned int index, const cds_removal_behavoir rb);

cds_result cds_dynamic_array_find(const cds_dynamic_array *array, const void *data, unsigned int *index);
cds_result cds_dynamic_array_find_cmp(const cds_dynamic_array *array, const void *data, unsigned int *index, const cds_cmp_func cmp_func);

cds_result cds_dynamic_array_iterate(const cds_dynamic_array *array, const cds_visit_func visit_func);
// sort methods...
#endif