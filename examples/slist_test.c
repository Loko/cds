#include "cds_slist.h"

int cmpInt(const void *a, const void *b) {
	if (a) {
		if (b) {
			int *pA = (int *)a;
			int *pB = (int *)b;
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
	if (ptr) {
		cds_log("%d ", *((int *)ptr));
	} else {
		cds_log("NULL");
	}
}

void logList(const cds_slist *list, int printSize) {
	if (list) {
		if (printSize)
			cds_log("Count: %d\n", list->count);
		cds_log("NULL");
		cds_slnode *cur = list->head;
		while(cur) {
			cds_log("<-%d->", *((int *)cur->data));
			cur = cur->next;
		}
		if (list->count)
			cds_log("NULL");
		cds_log("\n");
	}
}

void printLinkedList(cds_slist *list) {
    if (list && list->count) {
        cds_slnode *cur = cds_slist_head(list);
        while(cur) {
            int *pv = (int *) cur->data;
            printf("{a: %d d: %d n: %d}->", cur, *pv, cur->next);
            cur = cur->next;
        }
    }
    printf("NULL\n");
}

void pickNode(const cds_slist *list, unsigned int index, cds_slnode **node) {
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
	cds_log("Running the slist test...\n");
	
	/* first allocate space for the list */
	cds_slist *list = NULL;
	cds_result cr;
	cr = cds_slist_create(&list);
	if (cds_error_check(cr))
		return 1;
	
	/* add the 10 values to the slist */
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		cr = cds_slist_add_last(list, values + i);
		if (cds_error_check(cr))
			return 1;
	}
	
	/* print the list */
	logList(list, 1);
	
	/* functions are given to retrieve the head and tail node */
	cds_slnode *headNode = cds_slist_head(list);
	if (headNode)
		cds_log("Head Data: %d\n", *((int *)headNode->data));
	cds_slnode *tailNode = cds_slist_tail(list);
	if (tailNode)
		cds_log("Tail Data: %d\n", *((int *)tailNode->data));
	
	/* calls are given to remove the head/tail */
	cds_log("Remove the head and tail...\n");
	cr = cds_slist_remove_head(list);
	if (cds_error_check(cr))
		return 1;
	cr = cds_slist_remove_tail(list);
	if (cds_error_check(cr))
		return 1;
	
	
	logList(list, 1);
	
	cds_log("Trying the insert after/before functions...\n");
	unsigned int count = cds_slist_count(list);
	cds_slnode *midNode = NULL;
	/* this unofficial function simply picks a node for us to use for insertions */
	pickNode(list, count / 2, &midNode);
	
	/* inserting after is fast in a singly linked list */
	cds_log("Inserting Node with Value: 0 after the middle node...\n");
	cr = cds_slist_insert_after(list, midNode, &values[0]);
	if (cds_error_check(cr))
		return 1;
	logList(list, 0);
	cds_log("Inserting Node with Value: 9 after the middle node...\n");
	cds_slnode *nextNode = NULL;
	/* you can get back the new node when adding/inserting using the _node calls */
	cr = cds_slist_insert_after_node(list, midNode, &values[9], &nextNode);
	if (cds_error_check(cr))
		return 1;
	logList(list, 0);
	cds_log("Inserting a Node with Value: 3 after the last node that was inserted...\n");
	/* no searching needed, this is efficient */
	cr = cds_slist_insert_after(list, nextNode, &values[3]);
	if (cds_error_check(cr))
		return 1;
	logList(list, 0);
	
	cds_log("Attempting removal functions...\n");
	/* like with the other containers we can remove by address */
	cr = cds_slist_remove(list, &values[2]);
	
	int searchVal = 7;
	cds_log("Attempting remove by value with 7\n");
	/* and we can remove by value */
	cr = cds_slist_remove_cmp(list, &searchVal, &cmpInt);
	
	logList(list, 0);
	
	/* reverse the list */
	cds_log("For good measure let's reverse the singly linked list too...\n");
	cds_slist_reverse(list);
	
	/* use the iterate function */
	cds_log("Printing the reversed list using the iterate function...\n");
	cds_slist_iterate(list, &printInt);
	cds_log("\n");
	
	headNode = cds_slist_head(list);
	cds_log("Head: %d\n", *((int *) headNode->data));
	tailNode = cds_slist_tail(list);
	cds_log("Tail: %d\n", *((int *) tailNode->data));
	
	/* reverse the list */
	cds_log("Let's reverse the singly linked list again...\n");
	cds_slist_reverse(list);
	
	/* use the iterate function */
	cds_log("Printing the reversed list using the iterate function...\n");
	cds_slist_iterate(list, &printInt);
	cds_log("\n");
	
	headNode = cds_slist_head(list);
	cds_log("Head: %d\n", *((int *) headNode->data));
	tailNode = cds_slist_tail(list);
	cds_log("Tail: %d\n", *((int *) tailNode->data));
	
	/* delete the list */
	cds_log("Deleting the list...\n");
	cr = cds_slist_delete(&list);
	if (cds_error_check(cr)) 
		return 1;
	cds_log("Deletion successful...\n");
	return 0;
}