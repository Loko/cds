#include "cds_hash_node.h"

cds_result cds_hash_node_create(cds_hash_node **hashnode, void *data) {
	*node = (cds_hash_node *) cds_alloc(sizeof(cds_hash_node));
	if (*node) {
		(*node)->next = NULL;
		(*node)->data = data;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}


cds_result cds_hash_node_delete(cds_hash_node **hashnode) {
	if (*node) {
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}


cds_result cds_hash_node_delete_all(cds_hash_node **hashnode) {
	if (*node) {
		cds_free((*node)->data);
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}