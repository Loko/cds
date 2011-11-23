#include "cds_queue.h"

void printInt(const void *ptr) {
	if (ptr)
		cds_log("%d ", *(int *) ptr);
	else
		cds_log("NULL");
}

void logQueue(const cds_queue *queue, int printSize) {
	if (queue) {
		if (printSize)
			cds_log("Count: %u\n", cds_queue_count(queue));
		int *front = (int *)cds_queue_front(queue);
		cds_slnode *cur = NULL;
		if (front) {
			cds_log("Front: %d ", *front);
			cur = queue->front->next; /* want to start the loop at the second node */
		} else {
			cds_log("Front: NULL");
		}
		while (cur) {
			if (cur->data)
				cds_log("%d ", *((int *)cur->data));
			else
				cds_log("NULL");
			cur = cur->next;
		}
		cds_log(": Back\n");
	}
}

int main(int argc, char *argv[]) {
	/* Create the queue */
	cds_log("Running the queue test...\n");
	cds_queue *queue = NULL;
	cds_result cr = cds_queue_create(&queue);
	if (cds_error_check(cr))
		return 1;
	
	/* Add the usual values */
	cds_log("Adding the usual values with cds_queue_enqueue(queue, data)\n");
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	for (i = 0; i < n; ++i) {
		cr = cds_queue_enqueue(queue, values + i);
		if (cds_error_check(cr))
			return 1;
	}
	
	logQueue(queue, 1);
	
	/* dequeue until empty */
	int *front = NULL;
	while (cds_queue_count(queue)) {
		front = cds_queue_front(queue);
		cds_log("Dequeue: %d\n", *front); /* assuming no null entries */
		cr = cds_queue_dequeue(queue);
		if (cds_error_check(cr))
			return 1;
	}
	
	/* like the stack, when you dequeue and empty queue, you get CDS_UNDERFLOW returned */
	cds_log("The queue is now clear.  Count: %u\n", cds_queue_count(queue));
	cds_log("If you try to dequeue an empty stack, it will return: ");
	cr = cds_queue_dequeue(queue);
	char resultString[CDS_RESULT_MAX_STR_LEN];
	cds_result_string(cr, resultString);
	cds_log("%s\n", resultString);
	
	/* delete the queue */
	cds_log("Deleting the queue...\n");
	cr = cds_queue_delete(&queue);
	if (cds_error_check(cr))
		return 1;
	cds_log("Deletion successful...\n");
	return 0;
}