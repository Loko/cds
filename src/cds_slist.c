#include "cds_slist.h"

// 
cds_result cds_slist_create(cds_slist **list) {
	*list = (cds_slist *) cds_alloc(sizeof(cds_slist));
	if (*list) {
		(*list)->head = NULL;
		(*list)->tail = NULL;
		(*list)->count = 0;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

// 
cds_result cds_slist_delete(cds_slist **list) {
	if (list && *list) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = (*list)->head;
		while (node) {
			tmp = node->next;
			cds_slnode_delete(&node);
			node = tmp;
		}
		cds_free(*list);
		*list = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

// deletes the nodes but not the list or the data pointers referenced by the nodes
cds_result cds_slist_clear(cds_slist *list) {
	if (list) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = list->head;
		while (node) {
			tmp = node->next;
			cds_slnode_delete(&node);
			node = tmp;
		}
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

// deletes everything, the nodes, the data pointers in the nodes, and the list itself
cds_result cds_slist_delete_all(cds_slist **list) {
	if (list && *list) {
		cds_result r;
		cds_slnode *node, *tmp;
		node = (*list)->head;
		while (node) {
			tmp = node->next;
			cds_slnode_delete_all(&node);
			node = tmp;
		}
		//(*list)->head = NULL;
		//(*list)->tail = NULL;
		//(*list)->count = 0;
		cds_free(*list);
		*list = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
unsigned int cds_slist_count(const cds_slist *list) {
	unsigned int count = 0;
	if (list) {
		count = list->count;
	}
	return count;
}

//
cds_slnode* cds_slist_head(const cds_slist *list) {
	cds_slnode *head = NULL;
	if (list) {
		head = list->head;
	}
	return head;
}

// 
cds_slnode* cds_slist_tail(const cds_slist *list) {
	cds_slnode *tail = NULL;
	if (list) {
		tail = list->tail;
	}
	return tail;
}

//
cds_result cds_slist_add_first(cds_slist *list, const void *data) {
	if (!list)
		return CDS_NULL_ARGUMENT;
	cds_slnode *node;
	cds_result r = cds_slnode_create(&node, data);
	if (r == CDS_OK) {
		if (list->head) {
			node->next = list->head;
			list->head = node;
		} else {
			list->head = node;
			list->tail = node;
		}
		list->count++;
	}
	return r;
}

//
cds_result cds_slist_add_last(cds_slist *list, const void *data) {
	if (!list)
		return CDS_NULL_ARGUMENT;
	cds_slnode *node;
	cds_result r = cds_slnode_create(&node, data);
	if (r == CDS_OK) {
		if (list->tail) {
			list->tail->next = node;
			list->tail = node;
		} else {
			list->head = node;
			list->tail = node;
		}
		list->count++;
	}
	return r;
}

// 
cds_result cds_slist_insert_before(cds_slist *list, cds_slnode *node, const void *data) {
	if (!list || !node)
		return CDS_NULL_ARGUMENT;
	if (node == list->head) {
		return cds_slist_add_first(list, data);
	} else {
		cds_slnode *newnode;
		cds_result r = cds_slnode_create(&newnode, data);
		if (r == CDS_OK) {
			cds_slnode *tmp = list->head;
			while (tmp->next) {
				if (tmp->next == node) {
					newnode->next = tmp->next->next;
					tmp->next = newnode;
					list->count++;
					return CDS_OK;
				}
				tmp = tmp->next;
			}
		} else {
			return r;
		}
	}
	return CDS_OK;
}

// 
cds_result cds_slist_insert_before_node(cds_slist *list, cds_slnode *node, void *data, cds_slnode **cnode) {
	if (!list || !node)
		return CDS_NULL_ARGUMENT;
	if (node == list->head) {
		cds_result r = cds_slist_add_first(list, data);
		*cnode = (r == CDS_OK) ? list->head : NULL;
		return r;
	} else if (list->count >= 2) {
		cds_slnode *newnode;
		cds_result r = cds_slnode_create(&newnode, data);
		if (r == CDS_OK) {
			cds_slnode *tmp = list->head;
			while (tmp->next) {
				if (tmp->next == node) {
					newnode->next = tmp->next->next;
					tmp->next = newnode;
					*cnode = newnode;
					list->count++;
					return CDS_OK;
				}
				tmp = tmp->next;
			}
			return CDS_NOT_FOUND;
		} else {
			return r;
		}
	}
}

// 
cds_result cds_slist_insert_after(cds_slist *list, cds_slnode *node, const void *data) {
	if (!list || !node)
		return CDS_NULL_ARGUMENT;
	if (node == list->tail) {
		return cds_slist_add_last(list, data);
	} else {
		cds_slnode *newnode;
		cds_result r = cds_slnode_create(&newnode, data);
		if (r == CDS_OK) {
			newnode->next = node->next;
			node->next = newnode;
			list->count++;
		} else {
			return r;
		}
	}
	return CDS_OK;
}

//
cds_result cds_slist_insert_after_node(cds_slist *list, cds_slnode *node, const void *data, cds_slnode **cnode) {
	if (!list || !node) {
		return CDS_NULL_ARGUMENT;
	} else if (node == list->tail) {
		cds_result r = cds_slist_add_last(list, data);
		*cnode = (r == CDS_OK) ? list->tail : NULL;
		return r;
	} else {
		cds_slnode *newnode;
		cds_result r = cds_slnode_create(&newnode, data);
		if (r == CDS_OK) {
			newnode->next = node->next;
			node->next = newnode;
			*cnode = newnode;
			list->count++;
		} else {
			return r;
		}
	}
	return CDS_OK;
}

// 
cds_result cds_slist_remove_head(cds_slist *list) {
	if (!list)
		return CDS_NULL_ARGUMENT;
	if (list->count) {
		cds_slnode *tmp = list->head;
		list->head = list->head->next;
		if (list->head) {
			if (!list->head->next)
				list->tail = list->head;
		} else {
			list->tail = NULL;
		}
		cds_slnode_delete(&tmp);
		list->count--;
		return CDS_OK;
	} else {
		return CDS_UNDERFLOW;
	}
}

// 
cds_result cds_slist_remove_tail(cds_slist *list) {
	if (!list)
		return CDS_NULL_ARGUMENT;
	if (list->count) {
		cds_slnode *tmp = list->head, *tmpPrev = NULL;
		while (tmp->next) {
			tmpPrev = tmp;
			tmp = tmp->next;
		}
		cds_result r = cds_slnode_delete(&tmp);
		if (r != CDS_OK)
			return r;
		if (tmpPrev) {
			tmpPrev->next = NULL;
			list->tail = tmpPrev;
		} else {
			list->head = NULL;
			list->tail = NULL;
		}
		list->count--;
		if (list->count == 1) {
			list->head = list->tail;
		}
		return CDS_OK;
	} else {
		return CDS_UNDERFLOW;
	}
}

//
cds_result cds_slist_remove_head_data(cds_slist *list, void **data) {
	if (!list || !data)
		return CDS_NULL_ARGUMENT;
	if (list->count) {
		cds_slnode *tmp = list->head;
		list->head = list->head->next;
		if (list->head) {
			if (!list->head->next)
				list->tail = list->head;
		} else {
			list->tail = NULL;
		}
		*data = tmp->data;
		cds_slnode_delete(&tmp);
		list->count--;
		return CDS_OK;
	} else {
		return CDS_UNDERFLOW;
	}
}

//
cds_result cds_slist_remove_tail_data(cds_slist *list, void **data) {
	if (!list || !data)
		return CDS_NULL_ARGUMENT;
	if (list->count) {
		cds_slnode *tmp = list->head, *tmpPrev = NULL;
		while (tmp->next) {
			tmpPrev = tmp;
			tmp = tmp->next;
		}
		*data = tmp->data;
		cds_result r = cds_slnode_delete(&tmp);
		if (r != CDS_OK)
			return r;
		if (tmpPrev) {
			tmpPrev->next = NULL;
			list->tail = tmpPrev;
		} else {
			list->head = NULL;
			list->tail = NULL;
		}
		list->count--;
		if (list->count == 1) {
			list->head = list->tail;
		}
		return CDS_OK;
	} else {
		return CDS_UNDERFLOW;
	}
}

//
cds_result cds_slist_remove(cds_slist *list, const void *data) {
	if (!list) {
		return CDS_NULL_ARGUMENT;
	} else if (data == list->head->data) {
		return cds_slist_remove_head(list);
	} else if (data == list->tail->data) {
		return cds_slist_remove_tail(list);
	} else if (list->count > 2) {
		// already checked the head and tail
		// start with the second entry
		cds_slnode *node = list->head;
		CDS_ASSERT(node != NULL);
		while (node->next) {
			if (node->next->data == data) {
				// do the unlinking here
				cds_slnode *rnode = node->next;
				node->next = node->next->next;
				return cds_slnode_delete(&rnode);
			}
			node = node->next;
		}
	}
	return CDS_NOT_FOUND;
}

//
cds_result cds_slist_remove_node(cds_slist *list, cds_slnode *node) {
	if (!list || !node) {
		return CDS_NULL_ARGUMENT;
	} else if (node == list->head) {
		return cds_slist_remove_head(list);
	} else if (node == list->tail) {
		return cds_slist_remove_tail(list);
	} else if (list->count > 2) {
		// already checked the head and tail
		// start with the second entry
		cds_slnode *node = list->head;
		CDS_ASSERT(node != NULL);
		while (node->next) {
			if (node->next == node) {
				// do the unlinking here
				cds_slnode *rnode = node->next;
				node->next = node->next->next;
				list->count--;
				return cds_slnode_delete(&rnode);
			}
			node = node->next;
		}
	}
	return CDS_NOT_FOUND;
}

// 
cds_result cds_slist_remove_cmp(cds_slist *list, const void *data, const cds_cmp_func cmp_func) {
	if (!list || !cmp_func) {
		return CDS_NULL_ARGUMENT;
	} else if ((*cmp_func)(data, list->head->data) == 0) {
		return cds_slist_remove_head(list);
	} else if ((*cmp_func)(data, list->tail->data) == 0) {
		return cds_slist_remove_tail(list);
	} else if (list->count > 2) {
		// already checked the head and tail
		// start with the second entry
		cds_slnode *node = list->head;
		while (node) {
			if ((*cmp_func)(node->next->data, data) == 0) {
				// do the unlinking here
				cds_slnode *rnode = node->next;
				node->next = node->next->next;
				list->count--;
				return cds_slnode_delete(&rnode);
			}
			node = node->next;
		}
	}
	return CDS_NOT_FOUND;
}

//
cds_result cds_slist_find(const cds_slist *list, const void *data, cds_slnode **node) {
	if (!list || !node) {
		return CDS_NULL_ARGUMENT;
	} else if (data == list->head->data) {
		*node = list->head;
		return CDS_OK;
	} else if (data == list->tail->data) {
		*node = list->tail;
		return CDS_OK;
	} else if (list->count > 2) {
		// already checked the head and tail
		// start with the second entry
		cds_slnode *tmp = list->head->next;
		while (tmp) {
			if (tmp->data == data) {
				*node = tmp;
				return CDS_OK;
			}
			tmp = tmp->next;
		}
	}
	return CDS_NOT_FOUND;
}

//
cds_result cds_slist_find_cmp(const cds_slist *list, const void *data, cds_slnode **node, const cds_cmp_func cmp_func) {
	if (!list || !node || !cmp_func) {
		return CDS_NULL_ARGUMENT;
	} else if ((*cmp_func)(data, list->head->data) == 0) {
		*node = list->head;
		return CDS_OK;
	} else if ((*cmp_func)(data, list->tail->data) == 0) {
		*node = list->tail;
		return CDS_OK;
	} else if (list->count > 2) {
		// already checked the head and tail
		// start with the second entry
		cds_slnode *tmp = list->head->next;
		while (tmp) {
			if ((*cmp_func)(tmp->data, data) == 0) {
				*node = tmp;
				return CDS_OK;
			}
			tmp = tmp->next;
		}
	}
	return CDS_NOT_FOUND;
}

cds_result cds_slist_iterate(const cds_slist *list, const cds_visit_func visit_func) {
	if (list && visit_func) {
		cds_slnode *cur = list->head;
		while (cur) {
			(*visit_func)(cur->data);
			cur = cur->next;
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_slist_reverse(cds_slist *list) {
	if (list) {
		cds_slnode *newHead = NULL;
		cds_slnode *head = list->head;
		cds_slnode *newTail = list->head;
		while (head) {
			cds_slnode *next = head->next;
			head->next = newHead;
			newHead = head;
			head = next;
		}
		list->head = newHead;
		list->tail = newTail;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}