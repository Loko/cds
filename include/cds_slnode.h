/**
 * @file cds_slnode.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Singly Linked Node
 */

#ifndef _CDS_SLNODE_H_
#define _CDS_SLNODE_H_

 /** 
  * @addtogroup CDS_SLIST
  * @{
  */
#include "cds_core.h"

/**
 * A typical singly linked list node
 */
struct cds_slnode {
	struct cds_slnode *next;
	void *data;
};

/**  */
typedef struct cds_slnode cds_slnode;

/** Creates the singly linked list node */
cds_result cds_slnode_create(cds_slnode **node, const void *data);

/** Deletes the singly linked list node */
cds_result cds_slnode_delete(cds_slnode **node);

/** Deletes the singly linked list node and the data it points to */
cds_result cds_slnode_delete_all(cds_slnode **node);

/** @} */

#endif
