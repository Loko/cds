#include "cds_hash_node.h"

cds_result cds_hash_node_create(cds_hash_node **node, const void *key, const void *value) {
	*node = (cds_hash_node *) cds_alloc(sizeof(cds_hash_node));
	if (*node) {
		(*node)->next = NULL;
		(*node)->key = (void *) key;
		(*node)->value = (void *) value;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

cds_result cds_hash_node_delete(cds_hash_node **node, cds_hash_node_deletion_behavoir db) {
	if (*node) {
		if (db & CDS_DELETE_KEY)
			cds_free((*node)->key);
		if (db & CDS_DELETE_VALUE)
			cds_free((*node)->value);
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_hash_node_delete_all(cds_hash_node **node) {
	return cds_hash_node_delete(node, CDS_DELETE_ALL);
}
