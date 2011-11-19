#include "cds_binary_tree.h"

// NOTE: WHEN DELETING/CLEARING, DO IT WITH POST ORDER TRAVERSAL
// TODO: HANDLE MULTIPLE VALUES, FOR NOW DON'T ALLOW THEM

// 
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

//
cds_result cds_binary_tree_clear_recursive(cds_binary_node *root) {
	if (root) {
		cds_result r;
		cds_binary_node *left = root->left;
		cds_binary_node *right = root->right;
		r = cds_binary_node_delete(&root);
		if (r != CDS_OK)
			return r;
		
		if (left) {
			r = cds_binary_tree_clear_recursive(left);
			if (r != CDS_OK)
				return r;
			if (right) {
				r = cds_binary_tree_clear_recursive(right);
				if (r != CDS_OK)
					return r;
			}
		}
		return CDS_OK;
	}
}

//
cds_result cds_binary_tree_clear_all_recursive(cds_binary_node *root) {
	if (root) {
		cds_result r;
		cds_binary_node *left = root->left;
		cds_binary_node *right = root->right;
		r = cds_binary_node_delete_all(&root);
		if (r != CDS_OK)
			return r;
		
		if (left) {
			r = cds_binary_tree_clear_recursive(left);
			if (r != CDS_OK)
				return r;
			if (right) {
				r = cds_binary_tree_clear_recursive(right);
				if (r != CDS_OK)
					return r;
			}
		}
	}
	return CDS_OK;
}

// 
cds_result cds_binary_tree_delete(cds_binary_tree **tree) {
	if (tree && *tree) {
		cds_result r = cds_binary_tree_clear_recursive((*tree)->root);
		if (r == CDS_OK) {
			cds_free(*tree);
			*tree = NULL;
		}
		return r;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_binary_tree_clear(cds_binary_tree *tree) {
	// restore the count if the operation fails?
	if (tree) {
		return cds_binary_tree_clear_recursive(tree->root);
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_result cds_binary_tree_delete_all(cds_binary_tree **tree) {
	if (tree && *tree) {
		cds_result r = cds_binary_tree_clear_all_recursive((*tree)->root);
		if (r == CDS_OK) {
			cds_free(*tree);
			*tree = NULL;
		}
		return r;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

cds_binary_node * cds_binary_tree_root(const cds_binary_tree *tree) {
	cds_binary_node *root = NULL;
	if (tree) {
		root = tree->root;
	}
	return root;
}

unsigned int cds_binary_tree_count(const cds_binary_tree *tree) {
	unsigned int count = 0;
	if (tree) {
		count = tree->count;
	}
	return count;
}

cds_cmp_func cds_binary_tree_cmp_func(const cds_binary_tree *tree) {
	cds_cmp_func cmpFunc = NULL;
	if (tree) {
		cmpFunc = tree->cmp_func;
	}
	return cmpFunc;
}

void * cds_binary_tree_max(const cds_binary_tree *tree) {
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

// if the tree is null, but data pointer isn't, it would be good to set it to null
void * cds_binary_tree_min(const cds_binary_tree *tree) {
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

unsigned int cds_binary_tree_depth_recursive(cds_binary_node *root) {
	if (root) {
		unsigned int leftTreeHeight = cds_binary_tree_depth_recursive(root->left);
		unsigned int rightTreeHeight = cds_binary_tree_depth_recursive(root->right);
		return (leftTreeHeight > leftTreeHeight) ? leftTreeHeight + 1 : leftTreeHeight + 1;
	} else {
		return 0;
	}
}

unsigned int cds_binary_tree_height(const cds_binary_tree *tree) {
	unsigned int height = 0;
    if (tree && tree->count) {
        unsigned int leftHeight = cds_binary_tree_depth_recursive(tree->root->left);
		unsigned int rightHeight = cds_binary_tree_depth_recursive(tree->root->right);
        height = (leftHeight > rightHeight) ? leftHeight : rightHeight;
    }
    return height;
}

// pre order
void cds_binary_tree_preorder_recursive(cds_binary_node *root, cds_visit_func visit_func) {
	if (root) {
		(*visit_func)(root->data);
		cds_binary_tree_preorder_recursive(root->left, visit_func);
		cds_binary_tree_preorder_recursive(root->right, visit_func);
	}
}

cds_result cds_binary_tree_preorder_iterative(cds_binary_node *root, unsigned int count, cds_visit_func visit_func) {
	if (!root)
		return;
	cds_binary_node **nodeStack = (cds_binary_node **) cds_alloc(count * sizeof(cds_binary_node *));
	if (nodeStack) {
		unsigned int stackTop = 0;
		cds_binary_node *cur = root;
		while (1) {
			if (cur) {
				if (cur->right) {
					nodeStack[stackTop++] = cur->right;
				}
				nodeStack[stackTop++] = cur;
				cur = cur->left;
				continue;
			}
			
			if (!stackTop)
				return;
			
			cur = nodeStack[stackTop];
			--stackTop;
			
			if (cur->right && stackTop && cur->right == nodeStack[stackTop]) {
				--stackTop;
				nodeStack[stackTop] = cur->right;
			} else {
				// visit
				cur = NULL;
			}
		}
		
		cds_free(nodeStack);
	} else {
		return CDS_BAD_ALLOC;
	}
}

// in order
void cds_binary_tree_inorder_recursive(cds_binary_node *root, cds_visit_func visit_func) {
	if (root) {
		cds_binary_tree_inorder_recursive(root->left, visit_func);
		(*visit_func)(root->data);
		cds_binary_tree_inorder_recursive(root->right, visit_func);
	}
}

// post order
void cds_binary_tree_postorder_recursive(cds_binary_node *root, cds_visit_func visit_func) {
	if (root) {
		cds_binary_tree_postorder_recursive(root->left, visit_func);
		cds_binary_tree_postorder_recursive(root->right, visit_func);
		(*visit_func)(root->data);
	}
}

cds_result cds_binary_tree_breadthorder_traversal(const cds_binary_tree *tree, cds_visit_func visit_func) {
	// not illegal to try to traverse an empty binary tree
	// but need to check this case
	//if (!tree->count)
	//	return CDS_OK;
	
	// should be maxQueueSize / 2
	// but since we are moving up the front I think this is correct
	unsigned int maxQueueSize = tree->count;
	cds_binary_node **nodeQueue = (cds_binary_node **) cds_alloc(sizeof(cds_binary_node *) * maxQueueSize);
	unsigned int queueBack = 0;
	unsigned int queueFront = 0;

	if (*nodeQueue) {
		// probably need to nullify all those pointers??
		unsigned int i;
		for (i = 0; i < maxQueueSize; ++i) {
			printf("Address of pointer: %d\n", nodeQueue[i]);
			nodeQueue[i] = NULL;
		}
	
		// enqueue the root first
		nodeQueue[queueBack] = tree->root;
		++queueBack;
		
		while (queueBack != queueFront) {
			// dequeue and visit the front
			cds_binary_node *frontNode = nodeQueue[queueFront]; 
			(*visit_func)(frontNode->data);
			++queueFront;
			
			// queue left
			if (frontNode->left) {
				nodeQueue[queueBack] = frontNode->left;
				++queueBack;
			}
			// queue right
			if (frontNode->right) {
				nodeQueue[queueBack] = frontNode->right;
				++queueBack;
			}
		}
		
		cds_free(*nodeQueue);
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}


cds_result cds_binary_tree_insert(cds_binary_tree *tree, const void *data) {
	if (tree && data) {
		cds_binary_node *cur = tree->root;
		cds_binary_node *par = cur;
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

/*
cds_result cds_binary_tree_insert_node(cds_binary_tree *tree, const void *data, cds_binary_node **node, cds_binary_node **parent) {
	if (tree && data) {
		cds_binary_node *cur = tree->root;
		cds_binary_node *par = cur;
		while (cur) {
			int cmpResult = (*(tree->cmp_func))(cur->data, data);
			if (cmpResult == 0) {
				// this is a repeat value, right now I think I'll put off handling this
				*node = cur;
				*parent = par;
				return CDS_OK;
			} else if (cmpResult > 0) {
				par = cur;
				cur = par->left;
			} else {
				par = cur;
				cur = par->right;
			}
		}
		
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
			*node = nnode;
			*parent = par;
		} else {
			*node = NULL;
			*parent = NULL;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
*/

cds_result cds_binary_tree_remove(cds_binary_tree *tree, const void *data) {
	if (tree && data) {
		if (tree->root) {
			cds_binary_node *cur = tree->root;
			cds_binary_node *par = NULL;
			int cmpResult = (*(tree->cmp_func))(cur->data, data);
			while (cmpResult) {
				if (cmpResult > 0 ) {
					par = cur;
					cur = cur->left;
				} else if (cmpResult < 0) {
					par = cur;
					cur = cur->left;
				}
				
				if (cur)
					cmpResult = (*(tree->cmp_func))(cur->data, data);
				else
					return CDS_NOT_FOUND;
			}
			// handle cases here
			if (cur->right == NULL) {
				// case 1
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
				// case 2
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
				// case 3
				cds_binary_node *leftMost = cur->right->left;
				cds_binary_node *leftMostPar = cur->right;
				while (leftMost->left) {
					leftMostPar = cur->right;
					leftMost = leftMostPar->right->left;
				}
				leftMostPar->left = leftMost->right;
				leftMost->left = cur->left;
				leftMost->right = cur->right;
				if (par == NULL) {
					tree->root = cur;
				} else {
					cmpResult = (*(tree->cmp_func))(par->data, cur->data);
					if (cmpResult > 0 )
						par->left = leftMost;
					else if (cmpResult < 0)
						par->right = leftMost;
				}
			}
			
			return cds_binary_node_delete(&cur);
			
		} else {
			return CDS_NOT_FOUND;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}


/*
cds_result cds_binary_tree_remove_node(cds_binary_tree *tree, cds_binary_node *node, cds_binary_node *parent) {
	if (tree) {
		// tricky because the parent node could be null
		// be careful to handle every case here
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}

}
*/

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

// functions to get the depth of the tree
// ptr to the min and max values functions

//
cds_result cds_binary_tree_iterate(const cds_binary_tree *tree, cds_binary_tree_traversal_type traversal_type, cds_visit_func visit_func) {
	CDS_ASSERT_MSG(traversal_type == CDS_PRE_ORDER || traversal_type == CDS_IN_ORDER || traversal_type == CDS_POST_ORDER, "CDS Binary Tree Traversal Type Not Found!");
	if (tree && visit_func) {
			if (traversal_type == CDS_PRE_ORDER)
				cds_binary_tree_preorder_recursive(tree->root, visit_func);
			else if (traversal_type == CDS_IN_ORDER)
				cds_binary_tree_inorder_recursive(tree->root, visit_func);
			else if (traversal_type == CDS_POST_ORDER)
				cds_binary_tree_postorder_recursive(tree->root, visit_func);
			else
				cds_binary_tree_breadthorder_traversal(tree, visit_func);
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
