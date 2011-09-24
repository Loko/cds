#ifndef _CDS_DLNODE_H_
#define _CDS_DLNODE_H_

#include "cds_core.h"

//typedef struct cds_dlist cds_dlist;

struct cds_dlnode {
	struct cds_dlnode *next;
	struct cds_dlnode *prev;
	void *data; // double ptr?
	//cds_dlist *list;
	// a pointer to the list it belongs to??
	// it would solve the issue of removing nodes from other lists
	// this is accomplished here with a forwarded typedef
	// I feel this might also be a good idea for concurrency
};
typedef struct cds_dlnode cds_dlnode;

cds_result cds_dlnode_create(cds_dlnode **node, void *data);
cds_result cds_dlnode_delete(cds_dlnode **node);
cds_result cds_dlnode_delete_all(cds_dlnode **node);
#endif