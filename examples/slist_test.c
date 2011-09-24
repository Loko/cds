#include "cds_slist.h"

int intComparison(void *a, void *b) {
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

void printList(cds_slist *list) {
	if (list) {
		printf("\n----------\n");
		printf("Count: %d\n", list->count);		
		cds_slnode *cur = list->head;
		int *curData;
		while(cur) {
			curData = (int *) cur->data;
			printf("%d\n", *curData);
			cur = cur->next;
		}
		printf("----------\n");
	}
}

void pickNode(cds_slist *list, unsigned int index, cds_slnode **node) {
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
	printf("Running the singly linked list test...\n");
	cds_slist *list;
	cds_result cr; 
	
	cr = cds_slist_create(&list);
	if (cds_error_check(cr)) return 1;
	
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		cr = cds_slist_add_last(list, values + i);
		cds_error_check(cr);
	}
	
	int *tmp;
	printf("Count: %d\n", list->count);
	tmp = list->head->data;
	printf("Head: %d\n", *tmp);
	tmp = list->tail->data;
	printf("Tail: %d\n", *tmp);
	
	printList(list);
	
	//cds_dlnode *node;
	//pickNode(list, 3, &node);
	//cr = cds_dlist_insert_before(list, node, values + 9);
	//cds_error_check(cr);
	
	void *headData;
	cr = cds_slist_remove_head_data(list, &headData);
	cds_error_check(cr);
	printf("Head Data: %d", (*(int *)headData));
	
	printList(list);
	
	void *tailData;
	cr = cds_slist_remove_tail_data(list, &tailData);
	cds_error_check(cr);
	printf("Tail Data: %d\n", (*(int *)tailData));
	
	cr = cds_slist_remove_cmp(list, values + 5, &intComparison);
	if (cds_error_check(cr)) return 1;
	
	printList(list);
	
	cds_slnode *searchNode;
	cr = cds_slist_find_cmp(list, values + 8, &searchNode, &intComparison);
	if (cds_error_check(cr)) return 1;
	
	if (cr == CDS_OK) {
		cr = cds_slist_insert_after(list, searchNode, values + 9);
		if (cds_error_check(cr)) return 1;
	} else {
		printf("Not found!\n");
	}
	
	printList(list);
	
	cr = cds_slist_delete(&list);
	if (cds_error_check(cr)) return 1;
	
	/*
	printf("\nAttempt to print values:\n");
	//int *pi;
	for (i = 0; i < n; ++i) {
		printf("%d\n", values[i]);
	}
	*/
	
	return 0;
}