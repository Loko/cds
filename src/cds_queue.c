#include "cds_queue.h"

//
cds_result cds_queue_create(cds_queue **queue) {
	*queue = (cds_queue *) cds_alloc(sizeof(cds_queue));
	if (*queue) {
		(*queue)->head = NULL;
		(*queue)->tail = NULL;
		(*queue)->count = 0;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

//
cds_result cds_queue_delete(cds_queue **queue) {
	if (queue && *queue) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = (*queue)->head;
		while (node) {
			tmp = node->next;
			r = cds_slnode_delete(&node);
			if (r != CDS_OK)
				return r;
			node = tmp;
		}
		cds_free(*queue);
		*queue = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_queue_delete_all(cds_queue **queue) {
	if (queue && *queue) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = (*queue)->head;
		while (node) {
			tmp = node->next;
			r = cds_slnode_delete_all(&node);
			if (r != CDS_OK)
				return r;
			node = tmp;
		}
		cds_free(*queue);
		*queue = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_queue_clear(cds_queue *queue) {
	if (queue) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = queue->head;
		while (node) {
			tmp = node->next;
			r = cds_slnode_delete(&node);
			if (r != CDS_OK)
				return r;
			node = tmp;
		}
		queue->head = NULL;
		queue->tail = NULL;
		queue->count = 0;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

// like add tail in a singly linked list
cds_result cds_queue_enqueue(cds_queue *queue, void *data) {
	if (!queue)
		return CDS_NULL_ARGUMENT;
	cds_slnode *node;
	cds_result r = cds_slnode_create(&node, data);
	if (r == CDS_OK) {
		if (queue->tail) {
			queue->tail->next = node;
			queue->tail = node;
		} else {
			queue->head = node;
			queue->tail = node;
		}
		queue->count++;
	}
	return r;
}

// like remove head in a singly linked list
cds_result cds_queue_dequeue(cds_queue *queue) {
	if (!queue)
		return CDS_NULL_ARGUMENT;
	if (queue->count) {
		cds_slnode *tmp = queue->head;
		queue->head = queue->head->next;
		if (queue->head) {
			if (!queue->head->next)
				queue->tail = queue->head;
		} else {
			queue->tail = NULL;
		}
		cds_slnode_delete(&tmp);
		queue->count--;
		return CDS_OK;
	} else {
		return CDS_UNDERFLOW;
	}
}

// I think calling this function should return OK 
// even for an empty queue, in which case data is just set to null
cds_result cds_queue_front(cds_queue *queue, void **data) {
	if (!queue || !data)
		return CDS_NULL_ARGUMENT;
	if (queue->count) {
		*data = queue->head->data;
		return CDS_OK;
	} else {
		*data = NULL;
		return CDS_INVALID_OPERATION; // may very well be a different return, e.g. UNDERFLOW
	}
}

//
cds_result cds_queue_back(cds_queue *queue, void **data) {
	if (!queue || !data)
		return CDS_NULL_ARGUMENT;
	if (queue->count) {
		*data = queue->tail->data;
		return CDS_OK;
	} else {
		*data = NULL;
		return CDS_INVALID_OPERATION; // may very well be a different return, e.g. UNDERFLOW
	}
}