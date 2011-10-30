#ifndef _CDS_SLNODE_H_
#define _CDS_SLNODE_H_

#include "cds_core.h"

// a singly linked list node with a general purpose void pointer
struct cds_slnode {
	struct cds_slnode *next;
	void *data;
};
typedef struct cds_slnode cds_slnode;

cds_result cds_slnode_create(cds_slnode **node, void *data);
cds_result cds_slnode_delete(cds_slnode **node);
cds_result cds_slnode_delete_all(cds_slnode **node);

#endif