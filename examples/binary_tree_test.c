#include "cds_binary_tree.h"

int cmpInt(const void *a, const void *b) {
	if (a) {
		if (b) {
			int *pA = (int *)a;
			int *pB = (int *)b;
			if (*pA < *pB) {
				return -1;
			} else if (*pA > *pB) {
				return 1;
			} else {
				return 0;
			}
		} else {
			return 1;
		}
	} else {
		if (b)
			return -1;
		else
			return 0;
	}
}

void printInt(const void *v) {
	int *pi = (int *)v;
	if (pi) {
		cds_log("%d ", *pi);
	} else {
		cds_log("NULL ");
	}
}

void logTree(const cds_binary_tree *tree, int printSize, int printHeight) {
	if (printSize)
		cds_log("Tree Count: %u\n", cds_binary_tree_count(tree));
	if (printHeight)
		cds_log("Tree Height: %u\n", cds_binary_tree_height(tree));
	cds_log("Preorder Traversal: ");
	cds_binary_tree_iterate(tree, CDS_PRE_ORDER, &printInt);
	cds_log("\n");
	cds_log("InOrder Traversal: ");
	cds_binary_tree_iterate(tree, CDS_IN_ORDER, &printInt);
	cds_log("\n");
	cds_log("PostOrder Traversal: ");
	cds_binary_tree_iterate(tree, CDS_POST_ORDER, &printInt);
	cds_log("\n");
	cds_log("LevelOrder Traversal: ");
	cds_binary_tree_iterate(tree, CDS_LEVEL_ORDER, &printInt);
	cds_log("\n");
}

int main(int argc, char *argv[]) {
	/* Create the tree */
	cds_log("Running the binary tree test...\n");
	cds_binary_tree *tree = NULL;
	cds_result cr = cds_binary_tree_create(&tree, &cmpInt);
	if (cds_error_check(cr))
		return 1;
	
	/* Add the usual values */
	cds_log("Adding the usual values...\n");
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	/* insert takes the tree and the void pointer */
	/*			    5
	 *			3		 8
	 *		 2	  4   6     9
	 *	   1	        7
	 *
	 */
	cr = cds_binary_tree_insert(tree, values + 5); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 3); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 4); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 2); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 1); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 8); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 6); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 9); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 7); if (cds_error_check(cr)) return 1;
	
	/* reference the function to see how traversals can be done, as 
	 * well as functions for the count and height */
	logTree(tree, 1, 1);
	
	/* Getting the min and max values */
	cds_log("Getting the min and max values...\n");
	const int *min, *max;  /* const to indicate they shouldn't be changed */
	min = (const int *) cds_binary_tree_min(tree);
	max = (const int *) cds_binary_tree_max(tree);
	cds_log("Min: %d\n", *min);
	cds_log("Max: %d\n", *max);
	
	/* Make some removals */
	cds_log("Removing elements from the binary tree...\n");
	cds_log("Attempting removal of: %d\n", values[1]);
	cr = cds_binary_tree_remove(tree, &values[1]); /* leaf node */
	if (cds_error_check(cr)) return 1;
	
	cds_log("Attempting removal of: %d\n", values[8]);
	cr = cds_binary_tree_remove(tree, &values[8]);
	if (cds_error_check(cr)) return 1;
	
	/* note, comparisons are always done by value not address 
	 * there is no remove_cmp function to worry about, hence
	 * why this will work
	 */
	int valueOfRootNode = 5;
	cds_log("Attempting removal of: %d\n", valueOfRootNode);
	cr = cds_binary_tree_remove(tree, &valueOfRootNode);
	if (cds_error_check(cr)) return 1;
	
	logTree(tree, 1, 1);
	
	/* Clear the binary tree */
	cds_log("Attempting to clear the binary tree...\n");
	cr = cds_binary_tree_clear(tree);
	if (cds_error_check(cr)) return 1;
	cds_log("Tree cleared successfully...\n");
	logTree(tree, 1, 1);
	
	/* add some values again */
	cds_log("Adding some new values to the binary tree");
	cr = cds_binary_tree_insert(tree, values + 6); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 8); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 9); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 7); if (cds_error_check(cr)) return 1;
	cr = cds_binary_tree_insert(tree, values + 1); if (cds_error_check(cr)) return 1;
	/*
	 * show newly formed tree here
	 *
	 */
	
	logTree(tree, 1, 1);
	
	/* You can perform certain operations on subtrees too, which may be more efficient/desirable */
	cds_log("Searching for a subtree with value: %d\n", values[8]);
	cds_binary_node *node = NULL;
	cr = cds_binary_tree_find(tree, values + 8, &node);
	if (cr == CDS_OK) {
		cds_log("Preorder Of Subtree: ");
		cr = cds_binary_tree_iterate_node(node, CDS_PRE_ORDER, &printInt); /* can iterate over a subtree */
		cds_log("\n");
		if (cds_error_check(cr)) return 1;
		
		/* provide function which clears the subtree (problem is the count which is kept in the tree) */
	}
    

    /* Delete the binary tree */
	cds_log("Deleting the binary tree...\n");
	cr = cds_binary_tree_delete(&tree);
	if (cds_error_check(cr)) 
		return 1;
	cds_log("Deletion of the binary tree was successful...\n");
	return 0;
}
