#ifndef _CDS_BINARY_NODE_H_
#define _CDS_BINARY_NODE_H_

#include "cds_core.h"

struct cds_binary_node {
	struct cds_binary_node *left;
	struct cds_binary_node *right;
	void *data;
};

typedef struct cds_binary_node cds_binary_node;

cds_result cds_binary_node_create(cds_binary_node **node, void *data);
cds_result cds_binary_node_delete(cds_binary_node **node);
cds_result cds_binary_node_delete_all(cds_binary_node **node);

// put clear recursive and clear_all_recursive here??

#endif