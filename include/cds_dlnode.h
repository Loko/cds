/**
 * @file cds_dlnode.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Doubly Linked Node
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CDS_DLNODE_H_
#define _CDS_DLNODE_H_

/** 
 * @addtogroup CDS_DLIST
 * @{
 */
 
#include "cds_core.h"

/**
 * Typical doubly linked list node structure.  Stores a void 
 * data pointer and links to the next and prev nodes. 
 */
struct cds_dlnode {
	struct cds_dlnode *next;
	struct cds_dlnode *prev;
	void *data;
	/* cds_dlist *list; could add a reference to the list in the future */
};
/** @typedef cds_dlnode type */
typedef struct cds_dlnode cds_dlnode;

/** Creates the double linked list node with the given data pointer  */
cds_result cds_dlnode_create(cds_dlnode **node, const void *data);

/** Deletes the node */
cds_result cds_dlnode_delete(cds_dlnode **node);

/** Deletes the node and its data pointer */
cds_result cds_dlnode_delete_all(cds_dlnode **node);

/** @} */
#endif

#ifdef __cplusplus
}
#endif
