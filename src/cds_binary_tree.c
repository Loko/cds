#include "cds_binary_tree.h"
#include "cds_queue.h"

/* */ 
cds_result cds_binary_tree_create(cds_binary_tree **tree, cds_cmp_func cmp_func) {
	if (tree && cmp_func) {
		*tree = (cds_binary_tree *) cds_alloc(sizeof(cds_binary_tree));
		if (*tree) {
			(*tree)->root = NULL;
			(*tree)->cmp_func = cmp_func;
			(*tree)->count = 0;
			return CDS_OK;
		} else {
			return CDS_BAD_ALLOC;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* delete the subtree in post order */
static cds_result cds_binary_tree_clear_recursive(cds_binary_node *node) {
	if (node) {		
		if (node->left)
			return cds_binary_tree_clear_recursive(node->left);
		if (node->right)
			return cds_binary_tree_clear_recursive(node->right);
		return cds_binary_node_delete(&node);
	} else {
		return CDS_OK;
	}
}

/*
static int cds_binary_tree_clear_recursive(cds_binary_node *node) {
	if (node) {		
		if (node->left)
			return cds_binary_tree_clear_recursive(node->left);
		if (node->right)
			return cds_binary_tree_clear_recursive(node->right);
		return cds_binary_node_delete(&node);
	} else {
		return CDS_OK;
	}
}
*/

/* deletes the subtree and it's contained data in post order */
static cds_result cds_binary_tree_clear_all_recursive(cds_binary_node *node) {
	if (node) {		
		if (node->left)
			return cds_binary_tree_clear_all_recursive(node->left);
		if (node->right)
			return cds_binary_tree_clear_all_recursive(node->right);
		return cds_binary_node_delete_all(&node);
	} else {
		return CDS_OK;
	}
}

/* */
cds_result cds_binary_tree_delete(cds_binary_tree **tree) {
	if (tree && *tree) {
		cds_result cr = cds_binary_tree_clear_recursive((*tree)->root);
		if (cr == CDS_OK) {
			cds_free(*tree);
			*tree = NULL;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/*
cds_result cds_binary_tree_delete_subtree(cds_binary_tree *tree, cds_binary_node *sroot) {
	
}
*/

/* */
cds_result cds_binary_tree_clear(cds_binary_tree *tree) {
	if (tree) {
		cds_result cr = cds_binary_tree_clear_recursive(tree->root);
		if (cr == CDS_OK) {
			tree->count = 0;
			tree->root = NULL;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_binary_tree_delete_all(cds_binary_tree **tree) {
	if (tree && *tree) {
		cds_result cr = cds_binary_tree_clear_all_recursive((*tree)->root);
		if (cr == CDS_OK) {
			cds_free(*tree);
			*tree = NULL;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_binary_node * cds_binary_tree_root(const cds_binary_tree *tree) {
	cds_binary_node *root = NULL;
	if (tree) {
		root = tree->root;
	}
	return root;
}

/* */
unsigned int cds_binary_tree_count(const cds_binary_tree *tree) {
	unsigned int count = 0;
	if (tree) {
		count = tree->count;
	}
	return count;
}

/* */
cds_cmp_func cds_binary_tree_cmp_func(const cds_binary_tree *tree) {
	cds_cmp_func cmpFunc = NULL;
	if (tree) {
		cmpFunc = tree->cmp_func;
	}
	return cmpFunc;
}

/* pointer of min element, not the node itself */
const void * cds_binary_tree_min(const cds_binary_tree *tree) {
	void *minData = NULL;
	if (tree) {
		cds_binary_node *cur = tree->root;
		if (cur) {
			while (cur->left) {
				cur = cur->left;
			}
		}
		minData = cur->data;
	}
	return minData;
}

/* pointer of max element, not the node itself */
const void * cds_binary_tree_max(const cds_binary_tree *tree) {
	void *maxData = NULL;
	if (tree) {
		cds_binary_node *cur = tree->root;
		if (cur) {
			while (cur->right) {
				cur = cur->right;
			}
		}
		maxData = cur->data;
	}
	return maxData;
}

/* */
static unsigned int cds_binary_tree_height_recursive(const cds_binary_node *node) {
	if (node) {
		unsigned int leftTreeHeight = cds_binary_tree_height_recursive(node->left);
		unsigned int rightTreeHeight = cds_binary_tree_height_recursive(node->right);
		return (leftTreeHeight > rightTreeHeight) ? leftTreeHeight + 1 : rightTreeHeight + 1;
	} else {
		return 0;
	}
}

/* height */
unsigned int cds_binary_tree_height(const cds_binary_tree *tree) {
	unsigned int height = 0;
	if (tree && tree->root) {
		unsigned int leftHeight = cds_binary_tree_height_recursive(tree->root->left);
		unsigned int rightHeight = cds_binary_tree_height_recursive(tree->root->right);
		height = (leftHeight > rightHeight) ? leftHeight : rightHeight;
	}
	return height;
}


/* */
cds_result cds_binary_tree_insert(cds_binary_tree *tree, const void *data) {
	if (tree && data) {
		cds_binary_node *cur = tree->root;
		cds_binary_node *par = cur;
		/* search down the tree */
		while (cur) {
			int cmpResult = (*(tree->cmp_func))(cur->data, data);
			if (cmpResult == 0) {
				return CDS_DUPLICATE_VALUE;
			} else if (cmpResult > 0) {
				par = cur;
				cur = par->left;
			} else {
				par = cur;
				cur = par->right;
			}
		}
		/* Create the new node, and insert left or right */
		cds_binary_node *nnode;
		cds_result cr = cds_binary_node_create(&nnode, data);
		if (cr == CDS_OK) {
			if (par == NULL) {
				tree->root = nnode;
			} else {
				int parCmpResult = (*(tree->cmp_func))(par->data, data);
				if (parCmpResult > 0 )
					par->left = nnode;
				else
					par->right = nnode;
			}
			tree->count++;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_binary_tree_remove(cds_binary_tree *tree, const void *data) {
	if (tree && data) {
		if (tree->root) {
			cds_binary_node *cur = tree->root;
			cds_binary_node *par = NULL;
			int cmpResult = (*(tree->cmp_func))(cur->data, data);
			/* search down the tree */
			while (cmpResult) {
				if (cmpResult > 0 ) {
					par = cur;
					cur = cur->left;
				} else if (cmpResult < 0) {
					par = cur;
					cur = cur->right;
				}
				
				if (cur)
					cmpResult = (*(tree->cmp_func))(cur->data, data);
				else
					return CDS_NOT_FOUND;
			}
			/* handle cases here */
			if (cur->right == NULL) {
				/* case 1 */
				if (par == NULL) {
					tree->root = cur->left;
				} else {
					cmpResult = (*(tree->cmp_func))(par->data, cur->data);
					if (cmpResult > 0)
						par->left = cur->left;
					else if (cmpResult < 0)
						par->right = cur->left;
				}
			} else if (cur->right->left == NULL) {
				/* case 2 */
				cur->right->left = cur->left;
				if (par == NULL) {
					tree->root = cur->right;
				} else {
					cmpResult = (*(tree->cmp_func))(par->data, cur->data);
					if (cmpResult > 0)
						par->left = cur->right;
					else if (cmpResult < 0)
						par->right = cur->right;
				}
			} else {
				/* case 3 */
				cds_binary_node *leftMost = cur->right->left;
				cds_binary_node *leftMostPar = cur->right;
				while (leftMost->left) {
					leftMostPar = leftMost;
					leftMost = leftMost->left;
				}
				leftMostPar->left = leftMost->right;
				leftMost->left = cur->left;
				leftMost->right = cur->right;
				if (par == NULL) {
					tree->root = leftMost;
				} else {
					cmpResult = (*(tree->cmp_func))(par->data, cur->data);
					if (cmpResult > 0 )
						par->left = leftMost;
					else if (cmpResult < 0)
						par->right = leftMost;
				}
			}
			tree->count--;
			return cds_binary_node_delete(&cur);
		} else {
			return CDS_NOT_FOUND;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_binary_tree_find(const cds_binary_tree *tree, const void *data, cds_binary_node **node) {
	if (tree && data && node) {
		cds_binary_node *cur = tree->root;
		int cmpResult;
		while (cur && (cmpResult = (*(tree->cmp_func))(cur->data, data))) {
			if (cmpResult > 0) {
				cur = cur->left;
			} else {
				cur = cur->right;
			}
		}
		*node = cur;
		return (cur) ? CDS_OK : CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* functions that calculate path height and not just depth of a subtree? */

/* pre order */
void cds_binary_tree_preorder_recursive(const cds_binary_node *node, cds_visit_func visit_func) {
	if (node) {
		(*visit_func)(node->data);
		cds_binary_tree_preorder_recursive(node->left, visit_func);
		cds_binary_tree_preorder_recursive(node->right, visit_func);
	}
}

/* in order */
void cds_binary_tree_inorder_recursive(const cds_binary_node *node, cds_visit_func visit_func) {
	if (node) {
		cds_binary_tree_inorder_recursive(node->left, visit_func);
		(*visit_func)(node->data);
		cds_binary_tree_inorder_recursive(node->right, visit_func);
	}
}

/* post order */
void cds_binary_tree_postorder_recursive(const cds_binary_node *node, cds_visit_func visit_func) {
	if (node) {
		cds_binary_tree_postorder_recursive(node->left, visit_func);
		cds_binary_tree_postorder_recursive(node->right, visit_func);
		(*visit_func)(node->data);
	}
}

/* level order */
/** @todo I do not like the dependencies this forces, it makes builds annoying to have to reference cds_slnode.c and cds_queue.c
  * a better solution would do this using an array as if it was a queue, or define my own struct inside the function
  */
cds_result cds_binary_tree_levelorder_iterative(const cds_binary_node *node, cds_visit_func visit_func) {
	
	/* using a queue for level order traversal */
	cds_queue *nodeQueue = NULL;
	cds_result cr = cds_queue_create(&nodeQueue);
	if (cr != CDS_OK) return cr;
	
	/* enqueue the root */
	cds_binary_node *curNode = (cds_binary_node *) node;
	cr = cds_queue_enqueue(nodeQueue, curNode);
	if (cr != CDS_OK) return cr;
	
	/* 
	 * while queue != empty 
	 * dequeue and visit the node at the front of the queue
	 * queue left if not null, queue right if not null
	 */
	while ((curNode = (cds_binary_node *) cds_queue_front(nodeQueue)) != NULL) {
		cr = cds_queue_dequeue(nodeQueue);
		if (cr != CDS_OK) return cr;
		(*visit_func)(curNode->data);
		if (curNode->left) {
			cr = cds_queue_enqueue(nodeQueue, curNode->left);
			if (cr != CDS_OK) return cr;
		}
		if (curNode->right) {
			cr = cds_queue_enqueue(nodeQueue, curNode->right);
			if (cr != CDS_OK) return cr;
		}
	}
	
	cr = cds_queue_delete(&nodeQueue);
	if (cr != CDS_OK) return cr;
	
	return CDS_OK;
}

/* preorder, post order, inorder can be done iteratively too... */

cds_result cds_binary_tree_iterate_node(const cds_binary_node *node, cds_binary_tree_traversal_type traversal_type, cds_visit_func visit_func) {
	if (visit_func) {
		if (traversal_type == CDS_PRE_ORDER) {
			cds_binary_tree_preorder_recursive(node, visit_func);
		} else if (traversal_type == CDS_IN_ORDER) {
			cds_binary_tree_inorder_recursive(node, visit_func);
		} else if (traversal_type == CDS_POST_ORDER) {
			cds_binary_tree_postorder_recursive(node, visit_func);
		} else if (traversal_type == CDS_LEVEL_ORDER) {
			return cds_binary_tree_levelorder_iterative(node, visit_func);
		} else {
			return CDS_INVALID_ARGUMENT;
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_binary_tree_iterate(const cds_binary_tree *tree, cds_binary_tree_traversal_type traversal_type, cds_visit_func visit_func) {
	if (tree && visit_func) {
		return cds_binary_tree_iterate_node(tree->root, traversal_type, visit_func);
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
