#ifndef _CDS_HASH_NODE_H_
#define _CDS_HASH_NODE_H_

struct cds_hash_node {
	void *data;
	struct cds_hash_node *next;
};
typedef struct cds_hash_node cds_hash_node;

cds_result cds_hash_node_create(cds_hash_node **hashnode, void *data);
cds_result cds_hash_node_delete(cds_hash_node **hashnode);
cds_result cds_hash_node_delete_all(cds_hash_node **hashnode);

#endif