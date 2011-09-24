#include "cds_queue.h"

void printQueue(cds_queue *queue) {
	if (queue) {
		printf("\n----------\n");
		printf("Count: %d\n", queue->count);		
		cds_slnode *cur = queue->head;
		int *curData;
		while(cur) {
			curData = (int *) cur->data;
			printf("%d\n", *curData);
			cur = cur->next;
		}
		printf("----------\n");
	}
}

int main(int argc, char *argv[]) {
	cds_queue *queue;
	cds_result r = cds_queue_create(&queue);
	if (cds_error_check(r)) {
		return 1;
	}
	
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		r = cds_queue_enqueue(queue, values + i);
		if (cds_error_check(r)) {
			return 1;
		}
	}
	
	printQueue(queue);
	
	void *front;
	while (queue->count >= 5) {
		r = cds_queue_front(queue, &front);
		if (r == CDS_OK) {
			printf("Front: %d\n", *((int *) front));
			r = cds_queue_dequeue(queue);
			if (r != CDS_OK) {
				cds_error_check(r);
				break;
			}
		} else {
			if (cds_error_check(r))
				break;
		}
	}
	
	printQueue(queue);
	
	r = cds_queue_delete(&queue);
	if (cds_error_check(r)) {
		return 1;
	}
	
	return 0;
}