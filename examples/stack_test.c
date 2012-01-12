#include "cds_stack.h"

void printInt(const void *ptr) {
	int *ptrVal = (int *)ptr;
	printf("%d\n", *ptrVal);
}

void logStack(const cds_stack *stack, int printSize) {
	if (stack) {
		if (printSize)
			cds_log("Count: %u\n", cds_stack_count(stack));
		int *top = (int *)cds_stack_top(stack);
		cds_slnode *cur = NULL;
		if (top) {
			cds_log("Top: %d\n", *top);
			cur = stack->top->next; /* want to start the loop at the second node */
		} else {
			cds_log("Top: NULL\n");
		}
		while (cur) {
			if (cur->data)
				cds_log("     %d\n", *((int *)cur->data));
			else
				cds_log("     NULL\n");
			cur = cur->next;
		}
	}
}

int main(int argc, char *argv[]) {
	/* Create the stack */
	cds_log("Running the stack test...\n");
	cds_stack *stack = NULL;
	cds_result cr = cds_stack_create(&stack);
	if (cds_error_check(cr))
		return 1;
	
	/* Add the values to the stack */
	cds_log("Adding the usual values...\n");
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		cr = cds_stack_push(stack, values + i);
		if (cds_error_check(cr))
			return 1;
	}
	
	logStack(stack, 1);
	
	/* Clean up the stack */
	cds_log("Popping everything off the stack...\n");
	int *top = NULL;
	/* if you want to allow null values in your stack 
	 * use cds_stack_count() instead */
	while ((top = (int *) cds_stack_top(stack)) != NULL) {
		cds_log("Pop: %d\n", *top);
		cr = cds_stack_pop(stack);
		if (cds_error_check(cr))
			return 1;
	}
	
	/* if you call pop on an empty stack it returns CDS_UNDERFLOW */
	cds_log("The stack is now clear.  Count: %u\n", cds_stack_count(stack));
	cds_log("If you try to pop an empty stack, it will return: ");
	cr = cds_stack_pop(stack);
	char resultString[CDS_RESULT_MAX_STR_LEN];
	cds_result_string(cr, resultString);
	cds_log("%s\n", resultString);
	
	/* you can also iterate on a stack like all the other data structures 
	 * but we don't do that here
	 */
	cds_log("Deleting the stack...\n");
	cr = cds_stack_delete(&stack);
	if (cds_error_check(cr))
		return 1;
	cds_log("Deletion successful...\n");
	return 0;
}