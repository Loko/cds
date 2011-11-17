#ifndef _CDS_DLNODE_H_
#define _CDS_DLNODE_H_

#include "cds_core.h"

// would need a forwarded typedef if the 
// nodes contain a reference to the pointer
// typedef struct cds_dlist cds_dlist;

struct cds_dlnode {
	struct cds_dlnode *next;
	struct cds_dlnode *prev;
	void *data;
	//cds_dlist *list; could add this in the future
};
typedef struct cds_dlnode cds_dlnode;

cds_result cds_dlnode_create(cds_dlnode **node, const void *data);
cds_result cds_dlnode_delete(cds_dlnode **node);
cds_result cds_dlnode_delete_all(cds_dlnode **node);
#endif