#include "cds_stack.h"

void printInt(void *ptr) {
	int *ptrVal = (int *)ptr;
	printf("%d\n", *ptrVal);
}

void printStack(cds_stack *stack) {
	if (stack) {
		printf("\n----------\n");
		printf("Count: %d\n", stack->count);		
		cds_stack_iterate(stack, &printInt);
		printf("----------\n");
	}
}

int main(int argc, char *argv[]) {
	cds_stack *stack;
	cds_result r = cds_stack_create(&stack);
	if (cds_error_check(r)) {
		return 1;
	}
	
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		r = cds_stack_push(stack, values + i);
		if (cds_error_check(r)) {
			return 1;
		}
	}
	
	printStack(stack);
	
	void *top;
	while (cds_stack_count(stack)) {
		r = cds_stack_top(stack, &top);
		if (r == CDS_OK) {
			printf("Top: %d\n", *((int *) top));
			r = cds_stack_pop(stack);
			if (r != CDS_OK) {
				cds_error_check(r);
				break;
			}
		} else {
			if (cds_error_check(r))
				break;
		}
	}
	
	r = cds_stack_delete(&stack);
	if (cds_error_check(r)) {
		return 1;
	}
	
	return 0;
}