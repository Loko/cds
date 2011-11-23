#include "cds_hash_table.h"

/* */
cds_result cds_hash_table_create(cds_hash_table **table, unsigned int size, cds_cmp_func key_cmp, cds_cmp_func val_cmp, cds_hash_func hash_func) {
	if (table && key_cmp && val_cmp && hash_func) {
		if (size) {
			*table = (cds_hash_table *) cds_alloc(sizeof(cds_hash_table));
			if (*table) {
				(*table)->buckets = cds_alloc(size * sizeof(cds_hash_node *));
				if ((*table)->buckets) {
					unsigned int i;
					for (i = 0; i < size; ++i) {
						(*table)->buckets[i] = NULL;
					}
					(*table)->hash_func = hash_func;
					(*table)->key_cmp_func = key_cmp;
					(*table)->value_cmp_func = val_cmp;
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

/* */
cds_result cds_hash_table_delete(cds_hash_table **table, cds_hash_node_deletion_behavoir db) {
	if (table && *table) {
		cds_result cr = cds_hash_table_clear(*table, db);
		if (cr == CDS_OK) {
			cds_free((*table)->buckets);
			cds_free(*table);
			*table = NULL;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_hash_table_clear(cds_hash_table *table, cds_hash_node_deletion_behavoir db) {
	if (table) {
		unsigned int i, numDeleted = 0;
		cds_hash_node *cur, *next;
		cds_result cr;
		for (i = 0; i < table->size && numDeleted < table->count; ++i) {
			cur = table->buckets[i];
			while (cur) {
				next = cur->next;
				cr = cds_hash_node_delete(&cur, db);
				if (cr != CDS_OK)
					return cr;
				++numDeleted;
				cur = next;
			}
		}
		table->count = 0;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_hash_table_delete_all(cds_hash_table **table) {
	return cds_hash_table_delete(table, CDS_DELETE_ALL);
}

/* */
unsigned int cds_hash_table_count(const cds_hash_table *table) {
	unsigned int count = 0;
	if (table) {
		count = table->count;
	}
	return count;
}

/* */
unsigned int cds_hash_table_size(const cds_hash_table *table) {
	unsigned int size = 0;
	if (table) {
		size = table->size;
	}
	return size;
}

/* */
cds_cmp_func cds_hash_table_key_cmp_func(const cds_hash_table *table) {
	cds_cmp_func keyCmp = NULL;
	if (table) {
		keyCmp = table->key_cmp_func;
	}
	return keyCmp;
}

/* */
cds_cmp_func cds_hash_table_value_cmp_func(const cds_hash_table *table) {
	cds_cmp_func valCmp = NULL;
	if (table) {
		valCmp = table->value_cmp_func;
	}
	return valCmp;
}

/* */
float cds_hash_table_load_factor(const cds_hash_table *table) {
	float lf = 0.0f;
	if (table && table->size) {
		lf = ((float)table->count / (float)table->size);
	}
	return lf;
}

/* */
cds_result cds_hash_table_get(const cds_hash_table *table, const void *key, void **value) {
	if (table && key && value) {
		/** @todo pass in size to the hash function? */
		unsigned int bucketIndex = table->hash_func(key) % table->size;
		cds_hash_node *cur = table->buckets[bucketIndex];
		while (cur) {
			if (table->key_cmp_func(key, cur->key) == 0) {
				*value = cur->value;
				return CDS_OK;
			}
			cur = cur->next;
		}
		*value = NULL;
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_hash_table_set(cds_hash_table *table, const void *key, const void* value) {
	if (table && key && value) {
		unsigned int bucketIndex = table->hash_func(key) % table->size;
		cds_hash_node *cur = table->buckets[bucketIndex];
		while (cur) {
			if (table->key_cmp_func(key, cur->key) == 0) {
				cur->value = (void *) value;
				return CDS_OK;
			}
			cur = cur->next;
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
static cds_result cds_hash_table_add_internal(cds_hash_node **buckets, unsigned int size, cds_cmp_func key_cmp, cds_hash_func hash_func, void *key, void *value) {
	if (buckets && key && value) {
		unsigned int bucketIndex = hash_func(key) % size;
		cds_hash_node *nnode;
		cds_result cr;
		/* if a list exists in this bucket, we need to insert it in sorted order */
		if (buckets[bucketIndex]) {
			cds_hash_node *prev = NULL;
			cds_hash_node *cur;
			for (cur = buckets[bucketIndex]; cur != NULL; cur = cur->next) {
				int cmpResult = key_cmp(key, cur->key);
				/* we sort the keys in a given index in ascending order */
				if (cmpResult < 0) {
					cr = cds_hash_node_create(&nnode, key, value);
					if (cr != CDS_OK) {
						return cr;
					}
					if (prev) {
						prev->next = nnode;
					}
					nnode->next = cur;
					return CDS_OK;
				} else if (cmpResult == 0) {
					return CDS_DUPLICATE_VALUE;
				}
				prev = cur;
			}
			/* if we got here we need to add the new node to the end of the bucket list */
			cr = cds_hash_node_create(&nnode, key, value);
			if (cr != CDS_OK) {
				return cr;
			}
			prev->next = nnode;
			return CDS_OK;
		} else {
			/* otwerwise just set the head of bucket to be the new node */
			cr = cds_hash_node_create(&nnode, key, value);
			if (cr == CDS_OK) {
				buckets[bucketIndex] = nnode;
			}
			return cr;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
static cds_result cds_hash_table_resize(cds_hash_table *table) {
	/** @todo custom growth rates for hash tables */
	unsigned int nsize = (table->size) ? table->size * CDS_DEFAULT_HASH_TABLE_GROWTH_MULTIPLIER : CDS_DEFAULT_HASH_TABLE_SIZE;
	cds_hash_node **buckets = cds_alloc(nsize * sizeof(cds_hash_node));
	if (buckets) {
		unsigned int i;
		for (i = 0; i < nsize; ++i) {
			buckets[i] = NULL;
		}
		cds_result cr;
		const cds_hash_node *node;
		for (i = 0; i < table->size; ++i) {
			for (node = table->buckets[i]; node != NULL; node = node->next) {
				cr = cds_hash_table_add_internal(buckets, nsize, table->value_cmp_func, table->hash_func, node->key, node->value);
				if (cr != CDS_OK)
					return cr; /** @todo do we keep going?? how do we handle this exactly? */
			}
		}
		cds_free(table->buckets);
		table->buckets = buckets;
		table->size = nsize;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

/* */
cds_result cds_hash_table_add(cds_hash_table *table, const void *key, const void *value) {
	if (table && key && value) {
		/* if full, handle resizing by inserting everything again */
		cds_result cr;
		if (table->count == table->size) {
			cr = cds_hash_table_resize(table);
			if (cr != CDS_OK)
				return cr;
		}
		unsigned int bucketIndex = table->hash_func(key) % table->size;
		cds_hash_node *nnode;

		/* if a list exists in this bucket, we need to insert it in sorted order */
		if (table->buckets[bucketIndex]) {
			cds_hash_node *prev = NULL;
			cds_hash_node *cur;
			for (cur = table->buckets[bucketIndex]; cur != NULL; cur = cur->next) {
				int cmpResult = table->key_cmp_func(key, cur->key);
				/* we sort the keys in a given index in ascending order */
				if (cmpResult < 0) {
					cr = cds_hash_node_create(&nnode, key, value);
					if (cr != CDS_OK) {
						return cr;
					}
					if (prev) {
						prev->next = nnode;
					}
					nnode->next = cur;
					table->count++;
					return CDS_OK;
				} else if (cmpResult == 0) {
					return CDS_DUPLICATE_VALUE;
				}
				prev = cur;
			}
			/* if we got here we need to add the new node to the end of the bucket list */
			cr = cds_hash_node_create(&nnode, key, value);
			if (cr != CDS_OK) {
				return cr;
			}
			prev->next = nnode;
			return CDS_OK;
		} else {
			/* otherwise just set the head of bucket to be the new node */
			cr = cds_hash_node_create(&nnode, key, value);
			if (cr == CDS_OK) {
				table->buckets[bucketIndex] = nnode;
				table->count++;
			}
			return cr;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}


/* */
cds_result cds_hash_table_remove(cds_hash_table *table, const void *key, cds_hash_node_deletion_behavoir db) {
	if (table && key) {
		unsigned int bucketIndex = table->hash_func(key) % table->size;
		cds_hash_node *cur = table->buckets[bucketIndex];
		cds_hash_node *prev = NULL;
		int cmpResult;
		cds_result cr;
		while (cur != NULL) {
			cmpResult = table->key_cmp_func(key, cur->key);
			if (cmpResult < 0) {
				break; /* passed the point of possibly finding a key */
			} else if (cmpResult == 0) {
				if (prev) {
					prev->next = cur->next;
				} else {
					table->buckets[bucketIndex] = cur->next;
				}
				cr = cds_hash_node_delete(&cur, db);
				if (cr == CDS_OK) {
					table->count--;
				}
				return cr;
			}
			prev = cur;
			cur = cur->next;
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_hash_table_iterate_keys(const cds_hash_table *table, cds_visit_func visit_func) {
	if (table && visit_func) {
		unsigned int i;
		cds_hash_node *cur;
		for (i = 0; i < table->size; ++i) {
			cur = table->buckets[i];
			while (cur) {
				(*visit_func)(cur->key);
				cur = cur->next;
			}
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_hash_table_iterate_values(const cds_hash_table *table, cds_visit_func visit_func) {
	if (table && visit_func) {
		unsigned int i;
		cds_hash_node *cur;
		for (i = 0; i < table->size; ++i) {
			cur = table->buckets[i];
			while (cur) {
				(*visit_func)(cur->value);
				cur = cur->next;
			}
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_hash_table_iterate(const cds_hash_table *table, cds_visit_pair_func visit_pair_func) {
	if (table && visit_pair_func) {
		unsigned int numVisited = 0, i;
		cds_hash_node *cur = NULL;
		for (i = 0; i < table->size && numVisited < table->count; ++i) {
			cur = table->buckets[i];
			while (cur) {
				(*visit_pair_func)(cur->key, cur->value);
				++numVisited;
				cur = cur->next;
			}
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
