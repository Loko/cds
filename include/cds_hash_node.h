/**
 * @file cds_hash_node.h
 * @author Jeff Lansing
 * @brief A Collection of Functions for Hash Nodes
 */

#ifndef _CDS_HASH_NODE_H_
#define _CDS_HASH_NODE_H_

/** 
 * @addtogroup CDS_HASH_TABLE
 * @{
 */
 
/**
 * 
 *
 */
struct cds_hash_node {
	void *key;
	void *data;
	struct cds_hash_node *next;
};

/** */
typedef struct cds_hash_node cds_hash_node;

/** Creates a hashnode with the key/value pair */
cds_result cds_hash_node_create(cds_hash_node **hashnode, const void *key, const void *data);

/** Deletes the hashnode */
cds_result cds_hash_node_delete(cds_hash_node **hashnode);

/** Deletes the hashnode  */
cds_result cds_hash_node_delete_all(cds_hash_node **hashnode);

/** @} */
#endif
