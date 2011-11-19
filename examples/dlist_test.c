#include "cds_dlist.h"

int intComparison(const void *a, const void *b) {
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
    printf("%d\n", *pI);
}

void printList(const cds_dlist *list) {
	if (list) {
        printf("\n----------\n");
        printf("Count: %d\n", cds_dlist_count(list));
        cds_dlist_iterate(list, &printInt);
        printf("\n----------\n");
    }
    /*
    if (list) {
		printf("\n----------\n");
		printf("Count: %d\n", list->count);		
		cds_dlnode *cur = list->head;
		int *curData;
		while(cur) {
			curData = (int *) cur->data;
			printf("%d\n", *curData);
			cur = cur->next;
		}
		printf("----------\n");
	}
    */
}

void printLinkedList(cds_dlist *list) {
    if (list && list->count) {
        printf("NULL->");
        cds_dlnode *cur = cds_dlist_head(list);
        while(cur) {
            int *pv = (int *) cur->data;
            printf("<-{a: %d p:%d d: %d n: %d}->", cur, cur->prev, *pv, cur->next);
            cur = cur->next;
        }
    }
    printf("NULL\n");
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
	printf("Running the doubly linked list test...\n");
	printf("Size of cds_dlnode: %u\n", sizeof(cds_dlnode));
	printf("Size of cds_dlist: %u\n", sizeof(cds_dlist));
	cds_dlist *list;
	cds_result cr; 
	
	
	
	cr = cds_dlist_create(&list);
	if (cds_error_check(cr)) return 1;
	
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		cr = cds_dlist_add_last(list, values + i);
		cds_error_check(cr);
	}
	
	int *tmp;
	printf("Count: %d\n", list->count);
	tmp = list->head->data;
	printf("Head: %d\n", *tmp);
	tmp = list->tail->data;
	printf("Tail: %d\n", *tmp);
	
	printList(list);
	
    /*
	//cds_dlnode *node;
	//pickNode(list, 3, &node);
	//cr = cds_dlist_insert_before(list, node, values + 9);
	//cds_error_check(cr);
	*/
    
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
	
	cr = cds_dlist_delete(&list);
	if (cds_error_check(cr)) return 1;
	
	return 0;
}