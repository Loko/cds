#include "cds_hash_table.h"

cds_result cds_hash_table_create(cds_hash_table **table, unsigned int size, cds_cmp_func cmp_func, cds_hash_func hash_func) {
	if (table && cmp_func && hash_func) {
		if (size) {
			*table = (cds_hash_table *) cds_alloc(sizeof(cds_hash_table));
			if (*table) {
				(*table)->buckets = (cds_hash_node *) cds_alloc(sizeof(cds_hash_node *) * size);
				if ((*table)->buckets) {
					unsigned int i;
					for (i = 0; i < size; ++i) {
						(*table)->buckets[i] = NULL;
					}
					(*table)->hash_func = hash_func;
					(*table)->cmp_func = cmp_func;
					(*table)->count = 0;
					(*table)->size = size;
					return CDS_OK;
				} else {
					cds_free(*table);
					*table = NULL;
					return CDS_BAD_ALLOC;
				}
			} else {
				return CDS_BAD_ALLOC;
			}
		} else {
			return CDS_INVALID_ARGUMENT;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}


cds_result cds_hash_table_delete(cds_hash_table **table) {
	if (table && *table) {
		cds_free((*table)->buckets);
		cds_free(*table);
		*table = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_hash_table_clear(cds_hash_table *table) {
	if (table) {
		unsigned int i;
		for (i = 0; i < table->size; ++i) {
			cds_hash_node *cur = table->buckets[i];
			cds_result r;
			while (cur) {
				r = cds_hash_node_delete(&cur);
				if (r != CDS_OK)
					return r;
				cur = cur->next;
			}
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_hash_table_delete_all(cds_hash_table **table) {
	if (table && *table) {
		unsigned int i;
		for (i = 0; i < table->size; ++i) {
			cds_hash_node *cur = table->buckets[i];
			cds_result r;
			while (cur) {
				r = cds_hash_node_delete(&cur);
				if (r != CDS_OK)
					return r;
				cur = cur->next;
			}
		}
		cds_free((*table)->buckets);
		cds_free(*table);
		*table = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

unsigned int cds_hash_table_count(cds_hash_table *table) {
	unsigned int count = 0;
	if (table) {
		count = table->count;
	}
	return count;
	/*
	if (count) {
		if (table) {
			*count = table->count;
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

//
unsigned int cds_hash_table_size(cds_hash_table *table) {
	unsigned int size = 0;
	if (table) {
		size = table->size;
	}
	return size;
}

//
cds_result cds_hash_table_add(cds_hash_table *table, void *key, void *data) {
	if (table && data) {
		// growth behavoir not yet implemented
		unsigned int bucketIndex = table->hash_func(key) % table->size;
		cds_hash_node *nnode;
		cds_result cr;
		//void * found = NULL;
		//unsigned int added = 0;
		if (table->buckeys[bucketIndex]) {
			cds_hash_node *prev = NULL;
			cds_hash_node *cur;
			int cmpResult = table->cmp_func(cur->data, data);
			if (cmpResult == 0) {
				// change the pointer address
				cur->data = data;
			}
		} else {
			// set to head of bucket
			cr = cds_hash_node_create(&nnode);
			if (cr == CDS_OK) {
				table->buckets[bucketIndex] = nnode;
				table->count++;
			}
			return cr;
		}
		
		if (table->buckets[bucketIndex] == NULL) {
			/* An empty bucket */
			buckets[bucket] = hashnode_create(data);
			added = 1;
		} else {
			hashnode * current, * previous = NULL;
			for (current = buckets[bucket]; 
					current != NULL && !found && !added; current = current->next) {
				const int result = cmpfn(current->data, data);
				if (result == 0) {
					/* Changing an existing entry */
					found = current->data;
					current->data = data;
				}
				else if (result > 0) {
					/* Add before current */
					hashnode * node = hashnode_create(data);
					node->next = current;
					if (previous == NULL) {
						/* Adding at the front */
						buckets[bucket] = node;
					}
					else {
						previous->next = node;
					}
					added = 1;
				}
				previous = current;
			}
			if (!added && current == NULL) {
				/* Adding at the end */
				previous->next = hashnode_create(data);
			}
		}
		// find appropriate index
		// walk to appropriate place (with cmp func)
		// insert into the linked list
		// increment the count
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_hash_table_remove(cds_hash_table *table, void *key) {
	if (table && data) {
		unsigned int bucketIndex = table->hash_func(data) % table->count;
		cds_hash_node *cur = table->buckets[bucketIndex];
		cds_hash_node *prev = NULL;
		int cmpResult;
		while (cur != NULL) {
			cmpResult = table->cmp_func(cur->data, data);
			if (cmpResult == 0) {
				if (prev) {
					prev->next = cur->next;
				} else {
					table->buckets[bucketIndex] = cur->next;
				}
				cds_result cr = cds_hash_node_delete(&cur);
				if (cr == CDS_OK)
					table->count--;
				return cr;
			} else if (cmpResult > 0) {
				break;
			} else {
				prev = cur;
				cur = cur->next;
			}
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_hash_table_find(cds_hash_table *table, void *key, cds_hash_node **node) {
	if (node) {
		if (table && data) {
			cds_hash_node *cur;
			unsigned int bucketIndex = table->hash_func(data) % table->size;
			for (cur = table->buckets[bucketIndex]; cur != NULL; cur = cur->next) {
				int cmpResult = table->cmp_func(cur->data, data);
				if (result == 0) {
					*node = cur;
					return CDS_OK;
				} else if (result > 0) {
					break;
				}
			}
			*node = NULL;
			return CDS_NOT_FOUND;
		} else {
			*node = NULL;
			return CDS_NULL_ARGUMENT;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

//
cds_result cds_hash_table_iterate(cds_hash_table *table, cds_visit_func visit_func) {
	if (table && visit_func) {
		unsigned int i;
		for (i = 0; i < table->size; ++i) {
			cds_hash_node *cur = table->buckets[i];
			cds_result r;
			while (cur) {
				(*visit_func)(cur->data);
				cur = cur->next;
			}
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}