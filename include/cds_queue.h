#ifndef _CDS_QUEUE_H_
#define _CDS_QUEUE_H_

#include "cds_core.h"
#include "cds_slnode.h"


// internally it's just a singly linked list
// consider renaming to front and back
struct cds_queue {
	cds_slnode *front;
	cds_slnode *back;
	unsigned int count;
};

typedef struct cds_queue cds_queue;

cds_result cds_queue_create(cds_queue **queue);
cds_result cds_queue_delete(cds_queue **queue);
cds_result cds_queue_delete_all(cds_queue **queue);
cds_result cds_queue_clear(cds_queue *queue);

unsigned int cds_queue_count(const cds_queue *queue);
void * cds_queue_front(const cds_queue *queue);
void * cds_queue_back(const cds_queue *queue);

cds_result cds_queue_enqueue(cds_queue *queue, const void *data);
cds_result cds_queue_dequeue(cds_queue *queue);

// could potentially put a find method here...
cds_result cds_queue_iterate(const cds_queue *queue, const cds_visit_func visit_func);
#endif