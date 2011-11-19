#include "cds_binary_tree.h"

int dummyCmp(const void *a, const void *b) {
    return 0;
}

int intComparison(const void *a, const void *b) {
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

//int tabCounter = 1;
//int countSoFar = 0;
//int height = 3;
//int count = 9;
//int mod = 3;

void printInt(const void *v) {
	//int t = 0;
	//if (countSoFar) {
		//while (t < tabCounter) {
		//	printf("\t");
		//	++t;
		//}
	//}
	int *pi = (int *)v;
	printf("%d\n", *pi);
	//++countSoFar;
	//if ((countSoFar % mod) == 0)
	//	++tabCounter;
}

void printTree(const cds_binary_tree *tree) {
	//tabCounter = 0;
	cds_binary_tree_iterate(tree, CDS_IN_ORDER, &printInt);
}

int main(int argc, char *argv[]) {
	cds_binary_tree *tree;
	cds_result r = cds_binary_tree_create(&tree, &intComparison);
	if (cds_error_check(r)) {
		return 1;
	}
	
	int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	unsigned int n = 10;
	unsigned int i;
	unsigned int half = n / 2;
	r = cds_binary_tree_insert(tree, values + 5);
	r = cds_binary_tree_insert(tree, values + 3);
	r = cds_binary_tree_insert(tree, values + 4);
	r = cds_binary_tree_insert(tree, values + 2);
	r = cds_binary_tree_insert(tree, values + 1);
	r = cds_binary_tree_insert(tree, values + 8);
	r = cds_binary_tree_insert(tree, values + 6);
	r = cds_binary_tree_insert(tree, values + 9);
	r = cds_binary_tree_insert(tree, values + 7);
	/*
	for (i = 0; i < half; ++i) {
		r = cds_binary_tree_insert(tree, values + half + i);
		if (cds_error_check(r)) return 1;
		r = cds_binary_tree_insert(tree, values + half - i);
		if (cds_error_check(r)) return 1;
	}
	*/
    //cds_visit_func vf = &printInt;
	printTree(tree);
	
	/*
	void *top;
	while (tree->count >= 0) {
		r = cds_tree_top(tree, &top);
		if (r == CDS_OK) {
			printf("Top: %d\n", *((int *) top));
			r = cds_tree_pop(tree);
			if (r != CDS_OK) {
				cds_error_check(r);
				break;
			}
		} else {
			if (cds_error_check(r))
				break;
		}
	}
	*/
    
    unsigned int treeHeight = cds_binary_tree_height(tree);
    printf("Tree Count: %u\n", tree->count);
	printf("Tree Height: %u\n", treeHeight);
    
	r = cds_binary_tree_delete(&tree);
    //r = CDS_BAD_ALLOC;
	if (cds_error_check(r)) return 1;
    

	
	return 0;
}