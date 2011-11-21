/**
 * @file cds_hash_node.h
 * @author Jeff Lansing
 * @brief A Collection of Functions for Hash Nodes
 */

#ifndef _CDS_HASH_NODE_H_
#define _CDS_HASH_NODE_H_

/** */
enum cds_hash_node_deletion_behavoir {
	CDS_DELETE_NODE_ONLY = 0,
	CDS_DELETE_KEY = 1,
	CDS_DELETE_VALUE = 2,
	CDS_DELETE_ALL = 3
};

/** */
typedef enum cds_hash_node_deletion_behavoir cds_hash_node_deletion_behavoir;

/** 
 * @addtogroup CDS_HASH_TABLE
 * @{
 */

#include "cds_core.h"
 
/**
 * Struture for a key/value pair based hash node usable in a @see cds_hash_table
 * @todo How do we handle table removal in terms of deleting the key, value, both, all
 */
struct cds_hash_node {
	void *key;
	void *value;
	struct cds_hash_node *next;
};

/** */
typedef struct cds_hash_node cds_hash_node;

/** Creates a hashnode with the key/value pair */
cds_result cds_hash_node_create(cds_hash_node **hashnode, const void *key, const void *value);

/** Deletes the hashnode and key, value, neither, or both depending on given deletion behavoir */
cds_result cds_hash_node_delete(cds_hash_node **hashnode, cds_hash_node_deletion_behavoir db);

/** Deletes the hashnode, its key, and its value  */
cds_result cds_hash_node_delete_all(cds_hash_node **hashnode);

/** @} */
#endif
