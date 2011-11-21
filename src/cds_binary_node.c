#include "cds_binary_node.h"

/* */
cds_result cds_binary_node_create(cds_binary_node **node, const void *data) {
	*node = (cds_binary_node *) cds_alloc(sizeof(cds_binary_node));
	if (*node) {
		(*node)->left = NULL;
		(*node)->right = NULL;
		(*node)->data = (void *) data;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

/* */
cds_result cds_binary_node_delete(cds_binary_node **node) {
	if (*node) {
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_binary_node_delete_all(cds_binary_node **node) {
	if (*node) {
		cds_free((*node)->data);
		(*node)->data = NULL;
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
