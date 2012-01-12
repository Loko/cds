/**
 * @file cds_stack.h
 * @author Jeff Lansing
 * A last in first out stack represented internally as a singly linked list.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CDS_STACK_H_
#define _CDS_STACK_H_

/** 
 * @addtogroup CDS_STACK
 * @{
 */

#include "cds_slnode.h"

/**
 * Abstract queue data type.  Holds the top stack node and the count.
 * @todo add a find method?
 */
struct cds_stack {
	cds_slnode *top;
	unsigned int count;
};

/** @typedef cds_stack type */
typedef struct cds_stack cds_stack;

/** Creates the stack */
cds_result cds_stack_create(cds_stack **stack);

/** Deletes the stack */
cds_result cds_stack_delete(cds_stack **stack);

/** Deletes the stack and its elements */
cds_result cds_stack_delete_all(cds_stack **stack);

/** Clears the stack, but doesn't delete it, or its referenced elements */
cds_result cds_stack_clear(cds_stack *stack);

/** Gets the top of the stack */
void * cds_stack_top(const cds_stack *stack);

/** Gets the number of elements in the stack */
unsigned int cds_stack_count(const cds_stack *stack);

/** Pushes data onto the top of the stack */
cds_result cds_stack_push(cds_stack *stack, const void *data);

/** Pops the top element off of the stack */
cds_result cds_stack_pop(cds_stack *stack);

/** Iterates through every element in the stack */
cds_result cds_stack_iterate(const cds_stack *stack, cds_visit_func visit_func);

/** @} */
#endif

#ifdef __cplusplus
}
#endif 

