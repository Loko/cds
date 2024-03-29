#include "cds_dlist.h"

/* */
cds_result cds_dlist_create(cds_dlist **list) {
	*list = (cds_dlist *) cds_alloc(sizeof(cds_dlist));
	if (*list) {
		(*list)->head = NULL;
		(*list)->tail = NULL;
		(*list)->count = 0;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

/* */
cds_result cds_dlist_delete(cds_dlist **list) {
	if (list) {
		cds_result cr;
		cds_dlnode *node, *tmp;
		node = (*list)->head;
		while (node) {
			tmp = node->next;
			cr = cds_dlnode_delete(&node);
			if (cr != CDS_OK)
				return cr;
			node = tmp;
		}
		cds_free(*list);
		*list = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_clear(cds_dlist *list) {
	if (list) {
		cds_result cr;
		cds_dlnode *node, *tmp;
		node = list->head;
		while (node) {
			tmp = node->next;
			cr = cds_dlnode_delete(&node);
			if (cr != CDS_OK)
				return cr;
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

/* */
cds_result cds_dlist_delete_all(cds_dlist **list) {
	if (list) {
		cds_result cr;
		cds_dlnode *node, *tmp;
		node = (*list)->head;
		while (node) {
			tmp = node->next;
			cr = cds_dlnode_delete_all(&node);
			if (cr != CDS_OK)
				return cr;
			node = tmp;
		}
		cds_free(*list);
		*list = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
unsigned int cds_dlist_count(const cds_dlist *list) {
	unsigned int count = 0;
	if (list) {
		count = list->count;
	}
	return count;
}

/* */
cds_dlnode * cds_dlist_head(const cds_dlist *list) {
	cds_dlnode *head = NULL;
	if (list) {
		head = list->head;
	}
	return head;
}

/* */
cds_dlnode * cds_dlist_tail(const cds_dlist *list) {
	cds_dlnode *tail = NULL;
	if (list) {
		tail = list->tail;
	}
	return tail;
}

/* */
cds_result cds_dlist_add_first(cds_dlist *list, const void *data) {
	if (list) {
		cds_dlnode *node;
		cds_result cr = cds_dlnode_create(&node, data);
		if (cr == CDS_OK) {
			if (list->head) {
				list->head->prev = node;
				node->next = list->head;
				list->head = node;
			} else {
				list->head = node;
				list->tail = node;
			}
			list->count++;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_add_last(cds_dlist *list, const void *data) {
	if (list) {
		cds_dlnode *node;
		cds_result cr = cds_dlnode_create(&node, data);
		if (cr == CDS_OK) {
			if (list->tail) {
				list->tail->next = node;
				node->prev = list->tail;
				list->tail = node;
	
			} else {
				list->head = node;
				list->tail = node;
			}
			list->count++;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	} 
}

/* */
cds_result cds_dlist_insert_before(cds_dlist *list, cds_dlnode *node, const void *data) {
	if (list && node) {
		if (node == list->head) {
			return cds_dlist_add_first(list, data);
		} else {
			cds_dlnode *newnode;
			cds_result cr = cds_dlnode_create(&newnode, data);
			/** @todo Force node search defines need to be here */
			if (cr == CDS_OK) {
				newnode->prev = node->prev;
				node->prev->next = newnode;
				newnode->next = node;
				node->prev = newnode;
				list->count++;
			}
			return cr;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_insert_after(cds_dlist *list, cds_dlnode *node, const void *data) {
	if (list && node) {
		if (node == list->tail) {
			return cds_dlist_add_last(list, data);
		} else {
			cds_dlnode *newnode;
			cds_result cr = cds_dlnode_create(&newnode, data);
			/** @todo Force node search defines need to be here */
			if (cr == CDS_OK) {
				newnode->prev = node;
				node->next->prev = newnode;
				newnode->next = node->next;
				node->next = newnode;
				list->count++;
			}
			return cr;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_remove_head(cds_dlist *list) {
	if (list) {
		if (list->count) {
			cds_dlnode *tmp = list->head;
			list->head = list->head->next;
			if (list->head) {
				list->head->prev = NULL;
				if (list->head->next)
					list->head->next->prev = list->head;
				else
					list->tail = list->head;
			} else {
				list->tail = NULL;
			}
			cds_result cr = cds_dlnode_delete(&tmp);
			/** @todo decrement count if delete failed? */
			list->count--;
			return cr;
		} else {
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_remove_tail(cds_dlist *list) {
	if (list) {
		if (list->count) {
			cds_dlnode *tmp = list->tail;
			list->tail = list->tail->prev;
			if (list->tail) {
				list->tail->next = NULL;
				if (list->tail->prev)
					list->tail->prev->next = list->tail;
				else
					list->head = list->tail;
			} else {
				list->head = NULL;
			}
			cds_result cr = cds_dlnode_delete(&tmp);
			list->count--;
			return cr;
		} else {
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_remove_head_data(cds_dlist *list, void **data) {
	if (list) {
		if (list->count) {
			cds_dlnode *tmp = list->head;
			list->head = list->head->next;
			if (list->head) {
				list->head->prev = NULL;
				if (list->head->next)
					list->head->next->prev = list->head;
				else
					list->tail = list->head;
			} else {
				list->tail = NULL;
			}
			*data = tmp->data;
			cds_dlnode_delete(&tmp);
			list->count--;
			return CDS_OK;
		} else {
			data = NULL;
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_remove_tail_data(cds_dlist *list, void **data) {
	if (list) {
		if (list->count) {
			cds_dlnode *tmp = list->tail;
			list->tail = list->tail->prev;
			if (list->tail) {
				list->tail->next = NULL;
				if (list->tail->prev)
					list->tail->prev->next = list->tail;
				else
					list->head = list->tail;
			} else {
				list->head = NULL;
			}
			*data = tmp->data;
			cds_result cr = cds_dlnode_delete(&tmp);
			list->count--;
			return cr;
		} else {
			data = NULL;
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* ptr address comparison */
cds_result cds_dlist_remove(cds_dlist *list, const void *data) {
	if (!list) {
		return CDS_NULL_ARGUMENT;
	} else if (data == list->head->data) {
		return cds_dlist_remove_head(list);
	} else if (data == list->tail->data) {
		return cds_dlist_remove_tail(list);
	} else if (list->count > 2) {
		/* already checked the head and tail
		 * start with the second entry
		 */
		cds_dlnode *node = list->head->next;
		while (node) {
			if (node->data == data) {
				/* do the unlinking here */
				node->prev->next = node->next;
				node->next->prev = node->prev;
				return cds_dlnode_delete(&node);
			}
			node = node->next;
		}
	}
	return CDS_NOT_FOUND;
}

/* */
cds_result cds_dlist_remove_node(cds_dlist *list, cds_dlnode *node) {
    if (!list || !node) {
		return CDS_NULL_ARGUMENT;
	} else if (node == list->head) {
		return cds_dlist_remove_head(list);
	} else if (node == list->tail) {
		return cds_dlist_remove_tail(list);
	} else if (list->count > 2) {
		/* already checked the head and tail
		 * start with the second entry
		 */
#if (!CDS_DLIST_FORCE_NODE_SEARCH)
		node->prev->next = node->next;
		node->next->prev = node->prev;
		return cds_dlnode_delete(&node);
#else
		cds_dlnode *tmp = list->head->next;
		while (tmp) {
			if (node == tmp) {
				/* do the unlinking here */
				tmp->prev->next = tmp->next;
				tmp->next->prev = tmp->prev;
				return cds_dlnode_delete(&node);
			}
			tmp = tmp->next;
		}
#endif
	}
	return CDS_NOT_FOUND;
}

/* with custom compare function */
cds_result cds_dlist_remove_cmp(cds_dlist *list, const void *data, const cds_cmp_func cmp_func) {
	if (!list || !cmp_func) {
		return CDS_NULL_ARGUMENT;
	} else if ((*cmp_func)(data, list->head->data) == 0) {
		return cds_dlist_remove_head(list);
	} else if ((*cmp_func)(data, list->tail->data) == 0) {
		return cds_dlist_remove_tail(list);
	} else if (list->count > 2) {
		/* already checked the head and tail
		 * start with the second entry
		 */
		cds_dlnode *node = list->head->next;
		while (node) {
			if ((*cmp_func)(node->data, data) == 0) {
				/* do the unlinking here */
				node->prev->next = node->next;
				node->next->prev = node->prev;
				/* this should always return CDS_OK as a result of previous checks */
				cds_result cr = cds_dlnode_delete(&node);
				list->count--;
				return cr;
			}
			node = node->next;
		}
	}
	return CDS_NOT_FOUND;
}

/* */
cds_result cds_dlist_find(const cds_dlist *list, const void *data, cds_dlnode **node) {
	if (!list || !node) {
		return CDS_NULL_ARGUMENT;
	} else if (data == list->head->data) {
		*node = list->head;
		return CDS_OK;
	} else if (data == list->tail->data) {
		*node = list->tail;
		return CDS_OK;
	} else if (list->count > 2) {
		/* already checked the head and tail
		 * start with the second entry
		 */
		cds_dlnode *tmp = list->head->next;
		while (tmp) {
			if (tmp->data == data) {
				*node = tmp;
				return CDS_OK;
			}
			tmp = tmp->next;
		}
	}
	*node = NULL;
	return CDS_NOT_FOUND;
}

/* */
cds_result cds_dlist_find_cmp(const cds_dlist *list, const void *data, cds_dlnode **node, cds_cmp_func cmp_func) {
	if (!list || !node || !cmp_func) {
		return CDS_NULL_ARGUMENT;
	} else if ((*cmp_func)(data, list->head->data) == 0) {
		*node = list->head;
		return CDS_OK;
	} else if ((*cmp_func)(data, list->tail->data) == 0) {
		*node = list->tail;
		return CDS_OK;
	} else if (list->count > 2) {
		/* already checked the head and tail
		 * start with the second entry
		 */
		cds_dlnode *tmp = list->head->next;
		while (tmp) {
			if ((*cmp_func)(tmp->data, data) == 0) {
				*node = tmp;
				return CDS_OK;
			}
			tmp = tmp->next;
		}
	}
	*node = NULL;
	return CDS_NOT_FOUND;
}

/* */
cds_result cds_dlist_reverse(cds_dlist *list) {
	if (list) {
		cds_dlnode *cur = list->head;
		cds_dlnode *tmp = NULL;
		cds_dlnode *tail = list->head;
		while (cur) {
			tmp = cur->prev;
			cur->prev = cur->next;
			cur->next = tmp;
			cur = cur->prev;
		}
		if (tmp)
			list->head = tmp->prev;
		list->tail = tail;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dlist_iterate(const cds_dlist *list, cds_visit_func visit_func) {
	if (list && visit_func) {
		cds_dlnode *cur = list->head;
		while (cur) {
			(*visit_func)(cur->data);
			cur = cur->next;
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
