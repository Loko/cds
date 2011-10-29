#include "cds_queue.h"

//
cds_result cds_queue_create(cds_queue **queue) {
	*queue = (cds_queue *) cds_alloc(sizeof(cds_queue));
	if (*queue) {
		(*queue)->front = NULL;
		(*queue)->back = NULL;
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
		node = (*queue)->front;
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
		node = (*queue)->front;
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
		node = queue->front;
		while (node) {
			tmp = node->next;
			r = cds_slnode_delete(&node);
			if (r != CDS_OK)
				return r;
			node = tmp;
		}
		queue->front = NULL;
		queue->back = NULL;
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
		if (queue->back) {
			queue->back->next = node;
			queue->back = node;
		} else {
			queue->front = node;
			queue->back = node;
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
		cds_slnode *tmp = queue->front;
		queue->front = queue->front->next;
		if (queue->front) {
			if (!queue->front->next)
				queue->back = queue->front;
		} else {
			queue->back = NULL;
		}
		cds_slnode_delete(&tmp);
		queue->count--;
		return CDS_OK;
	} else {
		return CDS_UNDERFLOW;
	}
}

unsigned int cds_queue_count(cds_queue *queue) {
	unsigned int count = 0;
	if (queue) {
		count = queue->count;
	}
	return count;
	/*
	if (count) {
		if (queue) {
			*count = queue->count;
			return CDS_OK;
		} else {
			*count = 0;
			return CDS_NULL_ARGUMENT;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
	*/
}

// I think calling this function should return OK 
// even for an empty queue, in which case data is just set to null
cds_result cds_queue_front(cds_queue *queue, void **data) {
	if (!queue || !data)
		return CDS_NULL_ARGUMENT;
	if (queue->count) {
		*data = queue->front->data;
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
		*data = queue->back->data;
		return CDS_OK;
	} else {
		*data = NULL;
		return CDS_INVALID_OPERATION; // may very well be a different return, e.g. UNDERFLOW
	}
}

//
cds_result cds_queue_iterate(cds_queue *queue, cds_visit_func visit_func) {
	if (queue && visit_func) {
		cds_slnode *cur = queue->front;
		while (cur) {
			(*visit_func)(cur->data);
			cur = cur->next;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}