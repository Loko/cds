#include "cds_dlnode.h"

// allocates the node with the given data pointer
cds_result cds_dlnode_create(cds_dlnode **node, void *data) {
	//if (!node)
	//	return CDS_INVALID_ARGUMENT;
	*node = (cds_dlnode *) cds_alloc(sizeof(cds_dlnode));
	if (*node) {
		(*node)->next = NULL;
		(*node)->prev = NULL;
		(*node)->data = data;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

// frees the node
cds_result cds_dlnode_delete(cds_dlnode **node) {
	if (*node) {
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

// frees the node AND its data pointer
cds_result cds_dlnode_delete_all(cds_dlnode **node) {
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