#ifndef _CDS_STACK_H_
#define _CDS_STACK_H_

#include "cds_slnode.h"

// uses a singly linked list
struct cds_stack {
	cds_slnode *top;
	unsigned int count;
};
typedef struct cds_stack cds_stack;

cds_result cds_stack_create(cds_stack **stack);
cds_result cds_stack_delete(cds_stack **stack);
cds_result cds_stack_delete_all(cds_stack **stack);
cds_result cds_stack_clear(cds_stack *stack);

void * cds_stack_top(const cds_stack *stack);
unsigned int cds_stack_count(const cds_stack *stack);
// cds_stack_is_empty(cds_stack *stack)

cds_result cds_stack_push(cds_stack *stack, const void *data);
cds_result cds_stack_pop(cds_stack *stack);

cds_result cds_stack_iterate(const cds_stack *stack, const cds_visit_func visit_func);
// contains method?
#endif