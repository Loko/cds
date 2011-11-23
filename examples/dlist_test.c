#include "cds_dlist.h"

int cmpInt(const void *a, const void *b) {
	if (a) {
		if (b) {
			int *pA = (int *)a;
			int *pB = (int *)b;
			printf("Comparing: %d with %d\n", *pA, *pB);
			if (*pA < *pB) {
				return -1;
			} else if (*pA > *pB) {
				return 1;
			} else {
				return 0;
			}
		} else {
			return 1;
		}
	} else {
		if (b)
			return -1;
		else
			return 0;
	}
}

void printInt(const void *ptr) {
    int *pI = (int *) ptr;
    printf("%d ", *pI);
}

void logList(const cds_dlist *list, int printSize) {
	if (list) {
		if (printSize)
			cds_log("Count: %u\n", cds_dlist_count(list));
        cds_dlist_iterate(list, &printInt);
		cds_log("\n");
    }
}

void logLinkedList(const cds_dlist *list) {
    if (list && list->count) {
        cds_log("NULL->");
        cds_dlnode *cur = cds_dlist_head(list);
        while(cur) {
            int *pv = (int *) cur->data;
            cds_log("<-{a: %d p:%d d: %d n: %d}->", cur, cur->prev, *pv, cur->next);
            cur = cur->next;
        }
    }
    cds_log("NULL\n");
}

void pickNode(cds_dlist *list, unsigned int index, cds_dlnode **node) {
	if (list) {
		*node = list->head;
		unsigned int counter = 0;
		unsigned int target = (index < list->count) ? index : list->count;
		while (node && counter != target) {
			*node = (*node)->next;
			++counter;
		}
	}
}

int main(void) {
	/* Create the doubly linked list */
	cds_log("Running the dlist test...\n");
	cds_dlist *list = NULL;
	cds_result cr = cds_dlist_create(&list);
	if (cds_error_check(cr)) return 1;
	
	/* Add the usual values to the doubly linked list
 	 * Also see the add_first, add_after, and add_before functions
	 */
	cds_log("Adding the usual values to the dlist...\n");
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		cr = cds_dlist_add_last(list, values + i);
		if (cds_error_check(cr)) return 1;
	}
	
	logList(list, 1);
	
	/* functions are given to retrieve the head and tail node */
	cds_dlnode *headNode = cds_dlist_head(list);
	if (headNode)
		cds_log("Head Data: %d\n", *((int *)headNode->data));
	cds_dlnode *tailNode = cds_dlist_tail(list);
	if (tailNode)
		cds_log("Tail Data: %d\n", *((int *)tailNode->data));
		
	/* calls are given to remove the head/tail */
	cds_log("Remove the head and tail...\n");
	cr = cds_dlist_remove_head(list);
	if (cds_error_check(cr)) return 1;
	cr = cds_dlist_remove_tail(list);
	if (cds_error_check(cr)) return 1;
	
	logList(list, 1);
	
	/*
	int *tmp;
	printf("Count: %d\n", list->count);
	tmp = list->head->data;
	printf("Head: %d\n", *tmp);
	tmp = list->tail->data;
	printf("Tail: %d\n", *tmp);
	
	printList(list);
	*/
    /*
	//cds_dlnode *node;
	//pickNode(list, 3, &node);
	//cr = cds_dlist_insert_before(list, node, values + 9);
	//cds_error_check(cr);
	*/
    /*
	void *headData;
	cr = cds_dlist_remove_head_data(list, &headData);
	cds_error_check(cr);
	printf("Head Data: %d", (*(int *)headData));
	
	printList(list);
	
	void *tailData;
	cr = cds_dlist_remove_tail_data(list, &tailData);
	cds_error_check(cr);
	printf("Tail Data: %d", (*(int *)tailData));
	
	printList(list);
	
	cr = cds_dlist_remove_cmp(list, values + 4, &intComparison);
	if (cds_error_check(cr)) return 1;
	
	printList(list);
	
	cds_dlnode *rnode;
    pickNode(list, 4, &rnode);
    cr = cds_dlist_remove_node(list, rnode);
	
    printLinkedList(list);
    printf("Reversing the linked list...\n");
    cds_dlist_reverse(list);
    printLinkedList(list);
	*/
	
	headNode = cds_dlist_head(list);
	cds_log("Head: %d\n", *((int *) headNode->data));
	tailNode = cds_dlist_tail(list);
	cds_log("Tail: %d\n", *((int *)tailNode->data));
	
	/* reverse the list */
	cds_log("For good measure let's reverse the doubly linked list too...\n");
	cds_dlist_reverse(list);
	
	/* use the iterate function */
	cds_log("Printing the reversed list using the iterate function...\n");
	cds_dlist_iterate(list, &printInt);
	cds_log("\n");
	
	headNode = cds_dlist_head(list);
	cds_log("Head: %d\n", *((int *) headNode->data));
	tailNode = cds_dlist_tail(list);
	cds_log("Tail: %d\n", *((int *)tailNode->data));
	
	cds_log("Deleting the dlist...\n");
	cr = cds_dlist_delete(&list);
	if (cds_error_check(cr)) return 1;
	cds_log("Deletion successful\n");
	
	return 0;
}