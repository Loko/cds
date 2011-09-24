#include "cds_slnode.h"

cds_result cds_slnode_create(cds_slnode **node, void *data) {
	//if (node)
	//	return CDS_INVALID_ARGUMENT;
	*node = (cds_slnode *) cds_alloc(sizeof(cds_slnode));
	if (*node) {
		(*node)->next = NULL;
		(*node)->data = data;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

cds_result cds_slnode_delete(cds_slnode **node) {
	if (*node) {
		//cds_free((*node)->next);
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_slnode_delete_all(cds_slnode **node) {
	if (*node) {
		//cds_free((*node)->next);
		cds_free((*node)->data);
		cds_free(*node);
		*node = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}