#ifndef _CDS_BINARY_TREE_H_
#define _CDS_BINARY_TREE_H_

#include "cds_binary_node.h"

// tree traversal types
enum cds_binary_tree_traversal_type {
	CDS_PRE_ORDER = 0,
	CDS_IN_ORDER = 1,
	CDS_POST_ORDER = 2,
	CDS_LEVEL_ORDER = 3
};
typedef enum cds_binary_tree_traversal_type cds_binary_tree_traversal_type;

struct cds_binary_tree {
	cds_binary_node *root;
	unsigned int count;
	cds_cmp_func cmp_func;
};
typedef struct cds_binary_tree cds_binary_tree;

cds_result cds_binary_tree_create(cds_binary_tree **tree, cds_cmp_func cmp_func);
cds_result cds_binary_tree_delete(cds_binary_tree **tree);
cds_result cds_binary_tree_clear(cds_binary_tree *tree);
cds_result cds_binary_tree_delete_all(cds_binary_tree **tree);

cds_result cds_binary_tree_insert(cds_binary_tree *tree, void *data);
cds_result cds_binary_tree_insert_node(cds_binary_tree *tree, void *data, cds_binary_node **node, cds_binary_node **parent);

cds_result cds_binary_tree_remove(cds_binary_tree *tree, void *data);
cds_result cds_binary_tree_remove_node(cds_binary_tree *tree, cds_binary_node *node, cds_binary_node *parent);

cds_result cds_binary_tree_find(cds_binary_tree *tree, void *data, cds_binary_node **node);

cds_result cds_binary_tree_min(cds_binary_tree *tree, void **data);
cds_result cds_binary_tree_max(cds_binary_tree *tree, void **data);
cds_result cds_binary_tree_depth(cds_binary_tree *tree, unsigned int *depth);

cds_result cds_binary_tree_iterate(cds_binary_tree *tree, cds_binary_tree_traversal_type traversal_type, cds_visit_func visit_func);

#endif