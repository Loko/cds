#ifndef _CDS_QUEUE_H_
#define _CDS_QUEUE_H_

#include "cds_core.h"
#include "cds_slnode.h"


// internally it's just a singly linked list
// consider renaming to front and back
typedef struct {
	cds_slnode *head;
	cds_slnode *tail;
	unsigned int count;
} cds_queue;

cds_result cds_queue_create(cds_queue **queue);
cds_result cds_queue_delete(cds_queue **queue);
cds_result cds_queue_delete_all(cds_queue **queue);
cds_result cds_queue_clear(cds_queue *queue);

cds_result cds_queue_count(cds_queue *queue, unsigned int *count);
cds_result cds_queue_front(cds_queue *queue, void **data);
cds_result cds_queue_back(cds_queue *queue, void **data);

cds_result cds_queue_enqueue(cds_queue *queue, void *data);
cds_result cds_queue_dequeue(cds_queue *queue);


// could potentially put a find method here...
cds_result cds_queue_iterate(cds_queue *queue, cds_visit_func visit_func);
#endif