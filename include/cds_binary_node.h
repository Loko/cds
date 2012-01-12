/**
 * @file cds_binary_node.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Binary Node
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CDS_BINARY_NODE_H_
#define _CDS_BINARY_NODE_H_

 /**
  * @addtogroup CDS_BINARY_TREE
  * @{
  */
#include "cds_core.h"

/**
 * Typical binary node structure with left node, right 
 * node and a general purpose void pointer
 */
struct cds_binary_node {
	struct cds_binary_node *left;
	struct cds_binary_node *right;
	void *data;
};

/** @typedef cds_binary_node type */
typedef struct cds_binary_node cds_binary_node;

/** Creates a binary node with the given data */
cds_result cds_binary_node_create(cds_binary_node **node, const void *data);

/** Deletes the binary node */
cds_result cds_binary_node_delete(cds_binary_node **node);

/** Deletes the binary node and its data pointer */
cds_result cds_binary_node_delete_all(cds_binary_node **node);

/** @} */
#endif

#ifdef __cplusplus
}
#endif
