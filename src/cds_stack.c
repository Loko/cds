#include "cds_stack.h"

//
cds_result cds_stack_create(cds_stack **stack) {
	*stack = (cds_stack *) cds_alloc(sizeof(cds_stack));
	if (*stack) {
		(*stack)->top = NULL;
		(*stack)->count = 0;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

//
cds_result cds_stack_delete(cds_stack **stack) {
	if (stack && *stack) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = (*stack)->top;
		while (node) {
			tmp = node->next;
			r = cds_slnode_delete(&node);
			if (r != CDS_OK)
				return r;
			node = tmp;
		}
		cds_free(*stack);
		*stack = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_stack_delete_all(cds_stack **stack) {
	if (stack && *stack) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = (*stack)->top;
		while (node) {
			tmp = node->next;
			cds_slnode_delete_all(&node);
			node = tmp;
		}
		cds_free(*stack);
		*stack = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_stack_clear(cds_stack *stack) {
	if (stack) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = stack->top;
		while (node) {
			tmp = node->next;
			cds_slnode_delete(&node);
			node = tmp;
		}
		stack->top = NULL;
		stack->count = 0;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

// pushes the given data to the top of the stack
cds_result cds_stack_push(cds_stack *stack, void *data) {
	if (!stack)
		return CDS_NULL_ARGUMENT;
	cds_slnode *node;
	cds_result r = cds_slnode_create(&node, data);
	if (r == CDS_OK) {
		if (stack->top) {
			node->next = stack->top;
			stack->top = node;
		} else {
			stack->top = node;
			stack->top->next = NULL;
		}
		stack->count++;
	}
	return r;
}

// mimics remove head functionality of a singly linked list
cds_result cds_stack_pop(cds_stack *stack) {
	if (stack) {
		if (stack->count) {
			cds_slnode *tmp = stack->top;
			stack->top = stack->top->next;
			stack->count--;
			return cds_slnode_delete(&tmp);
		} else {
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_stack_top(cds_stack *stack, void **data) {
	if (!stack || !data)
		return CDS_NULL_ARGUMENT;
	if (stack->count) {
		*data = stack->top->data;
		return CDS_OK;
	} else {
		*data = NULL;
		return CDS_UNDERFLOW; // may very well be a different return, e.g. CDS_INVALID_OPERATION
	}
}

//
unsigned int cds_stack_count(cds_stack *stack) {
	unsigned int count = 0;
	if (stack) {
		count = stack->count;
	}
	return count;
	
	/*
	if (count) {
		if (stack) {
			*count = stack->count;
			return CDS_OK;
		} else {
			*count = 0;
			return CDS_NULL_ARGUMENT;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
	*/
}

//
cds_result cds_stack_iterate(cds_stack *stack, cds_visit_func visit_func) {
	if (stack && visit_func) {
		cds_slnode *cur = stack->top;
		while (cur) {
			(*visit_func)(cur->data);
			cur = cur->next;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}