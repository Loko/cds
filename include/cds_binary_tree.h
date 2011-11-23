/**
 * @file cds_binary_tree.h
 * @author Jeff Lansing
 * @brief A Collection of Binary Search Tree Functions
 * No repeat values are allowed left < x < right
 * A @see cds_cmp_func is stored in the tree struct itself
 */

#ifndef _CDS_BINARY_TREE_H_
#define _CDS_BINARY_TREE_H_

/** 
 * @addtogroup CDS_BINARY_TREE
 * @{
 */
 
#include "cds_binary_node.h"
/* #include "cds_stack.h" used for breadth first search */

/**
 * @enum cds_binary_tree_traversal_type
 * tree traversal types (in order, pre order, post order)
 */
enum cds_binary_tree_traversal_type {
	CDS_PRE_ORDER = 0, 	/**< Pre Order Traversal */
	CDS_IN_ORDER = 1,  	/**< In Order Traversal */
	CDS_POST_ORDER = 2,	/**< Post Order Traversal */
	CDS_LEVEL_ORDER = 3 /**< Level Order aka Breadth First */
};

/**
 * @typedef traversal tree type
 */
typedef enum cds_binary_tree_traversal_type cds_binary_tree_traversal_type;

/**
 * @struct cds_binary_tree
 * Container for binary search trees.  A typical tree 
 * structure, with a root and count note how we keep 
 * the compare func pointer in the tree structure itself.  
 * Justified since a binary tree really shouldn't change 
 * contained types at runtime.
 * @todo Handle multiple values in the binary search tree
 */
struct cds_binary_tree {
	cds_binary_node *root;
	unsigned int count;
	cds_cmp_func cmp_func; /** @todo make constant? */
};

/** Type for the binary tree structure */
typedef struct cds_binary_tree cds_binary_tree;

/** 
 * Creates the binary tree with the given cmp_func 
 * @param tree tree pointer passed by reference
 * @param cmp_func comparison func pointer used for the tree
 * @return CDS_OK or CDS_BAD_ALLOC
 */
cds_result cds_binary_tree_create(cds_binary_tree **tree, cds_cmp_func cmp_func);

/** Deletes the binary tree */
cds_result cds_binary_tree_delete(cds_binary_tree **tree);

/** Clears the binary tree, deleting the nodes, without deleting the data pointers themselves */
cds_result cds_binary_tree_clear(cds_binary_tree *tree);

/** Deletes all the nodes and their data pointers */
cds_result cds_binary_tree_delete_all(cds_binary_tree **tree);

/** Gets the root node of the tree or NULL if the tree is null/empty */
cds_binary_node * cds_binary_tree_root(const cds_binary_tree *tree);

/** Gets the number of nodes in the tree or 0 if the tree is null/empty */
unsigned int cds_binary_tree_count(const cds_binary_tree *tree);

/** Gets the comparison function used by the tree */
cds_cmp_func cds_binary_tree_cmp_func(const cds_binary_tree *tree);

/** 
 * Gets the minimum value data element
 * @return The min element data pointer, or NULL if the tree is empty
 */
const void * cds_binary_tree_min(const cds_binary_tree *tree);

/** 
 * Gets the maximum value data element 
 * @return The max element data pointer, or NULL if the tree is empty
 */
const void * cds_binary_tree_max(const cds_binary_tree *tree);

/**
 * The height/depth of the tree.  Will return the max of the left/right subtree.
 * @return The height of the tree, a single root node has a height of zero
 */
unsigned int cds_binary_tree_height(const cds_binary_tree *tree);

/** Inserts the data pointer into a new node of the tree */
cds_result cds_binary_tree_insert(cds_binary_tree *tree, const void *data);

/** Removes the node with the given data pointer value */
cds_result cds_binary_tree_remove(cds_binary_tree *tree, const void *data);

/** Finds the node with the given pointer */
cds_result cds_binary_tree_find(const cds_binary_tree *tree, const void *data, cds_binary_node **node);

/** Iterates over every element in the node subtree using the given @see cds_binary_tree_traversal_type and @see cds_visit_func */
cds_result cds_binary_tree_iterate_node(const cds_binary_node *node, cds_binary_tree_traversal_type traversal_type, cds_visit_func visit_func);

/** Safely iterates over every element in the tree using the given @see cds_binary_tree_traversal_type and @see cds_visit_func */
cds_result cds_binary_tree_iterate(const cds_binary_tree *tree, cds_binary_tree_traversal_type traversal_type, cds_visit_func visit_func);

/** @} */

#endif
