#ifndef _CDS_STACK_H_
#define _CDS_STACK_H_

#include "cds_slnode.h"

// probably gonna use a singly linked list
typedef struct {
	cds_slnode *head;
	unsigned int count;
} cds_stack;

cds_result cds_stack_create(cds_stack **stack);
cds_result cds_stack_delete(cds_stack **stack);
cds_result cds_stack_delete_all(cds_stack **stack);
cds_result cds_stack_clear(cds_stack *stack);

cds_result cds_stack_push(cds_stack *stack, void *data);
cds_result cds_stack_pop(cds_stack *stack);
cds_result cds_stack_top(cds_stack *stack, void **data);

#endif