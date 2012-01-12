/**
 * @file cds_queue.h
 * @author Jeff Lansing
 * A first in first out queue data structure.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CDS_QUEUE_H_
#define _CDS_QUEUE_H_

/** 
 * @addtogroup CDS_QUEUE
 * @{
 */

#include "cds_slnode.h"

 
/**
 * Abstract queue data type.  Holds front and back pointers and the count.
 */
struct cds_queue {
	cds_slnode *front;
	cds_slnode *back;
	unsigned int count;
};

/** @typedef cds_queue type */
typedef struct cds_queue cds_queue;

/** Create the queue */
cds_result cds_queue_create(cds_queue **queue);

/** Deletes the queue */
cds_result cds_queue_delete(cds_queue **queue);

/** Deletes the queue and the data element pointers */
cds_result cds_queue_delete_all(cds_queue **queue);

/** Clears the queue but doesn't delete any referenced data */
cds_result cds_queue_clear(cds_queue *queue);

/** Gets the number of elements in the queue */
unsigned int cds_queue_count(const cds_queue *queue);

/** Gets a pointer to the first element in the queue */
void * cds_queue_front(const cds_queue *queue);

/** Gets a pointer to the last element in the queue */
void * cds_queue_back(const cds_queue *queue);

/** Adds the data onto the end of the queue */
cds_result cds_queue_enqueue(cds_queue *queue, const void *data);

/** Removes the element at the front of the queue */
cds_result cds_queue_dequeue(cds_queue *queue);

/** Safely iterates through every element in the queue */
cds_result cds_queue_iterate(const cds_queue *queue, cds_visit_func visit_func);

/** @} */
#endif

#ifdef __cplusplus
}
#endif
