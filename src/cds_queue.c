#include "cds_queue.h"

/* */
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

/* */
cds_result cds_queue_delete(cds_queue **queue) {
	if (queue && *queue) {
		cds_result cr;
		cds_slnode *node, *tmp;
		node = (*queue)->front;
		while (node) {
			tmp = node->next;
			cr = cds_slnode_delete(&node);
			if (cr != CDS_OK)
				return cr;
			node = tmp;
		}
		cds_free(*queue);
		*queue = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_queue_delete_all(cds_queue **queue) {
	if (queue && *queue) {
		cds_result cr;
		cds_slnode *node, *tmp;
		node = (*queue)->front;
		while (node) {
			tmp = node->next;
			cr = cds_slnode_delete_all(&node);
			if (cr != CDS_OK)
				return cr;
			node = tmp;
		}
		cds_free(*queue);
		*queue = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_queue_clear(cds_queue *queue) {
	if (queue) {
		cds_result cr;
		cds_slnode *node, *tmp;
		node = queue->front;
		while (node) {
			tmp = node->next;
			cr = cds_slnode_delete(&node);
			if (cr != CDS_OK)
				return cr;
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

/* */
cds_result cds_queue_enqueue(cds_queue *queue, const void *data) {
	if (queue) {
		cds_slnode *node;
		cds_result cr = cds_slnode_create(&node, data);
		if (cr == CDS_OK) {
			if (queue->back) {
				queue->back->next = node;
				queue->back = node;
			} else {
				queue->front = node;
				queue->back = node;
			}
			queue->count++;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_queue_dequeue(cds_queue *queue) {
	if (queue) {
		if (queue->front) {
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
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
unsigned int cds_queue_count(const cds_queue *queue) {
	unsigned int count = 0;
	if (queue) {
		count = queue->count;
	}
	return count;
}

/* */
void * cds_queue_front(const cds_queue *queue) {
	void *frontData = NULL;
	if (queue && queue->front) {
		frontData = queue->front->data;
	}
	return frontData;
}

/* */
void * cds_queue_back(const cds_queue *queue) {
	void *backData = NULL;
	if (queue && queue->back) {
		backData = queue->back->data;
	}
	return backData;
}

/* */
cds_result cds_queue_iterate(const cds_queue *queue, cds_visit_func visit_func) {
	if (queue && visit_func) {
		cds_slnode *cur = queue->front;
		while (cur) {
			(*visit_func)(cur->data);
			cur = cur->next;
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
