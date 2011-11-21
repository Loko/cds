#include "cds_hash_table.h"
#include "stdio.h"
#include "string.h"

unsigned int naiveIntHash(const void *key) {
	int *ikey = (int *) key;
	*ikey = (*ikey) < 0 ? -(*ikey) : *ikey;
	return (unsigned int)(*ikey);
}

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

int cmpStr(const void *a, const void *b) {
	char *s1 = (char *) a;
	char *s2 = (char *) b;
	char c1, c2;
	int v;
	do {
		c1 = (unsigned int) *s1++;
		c2 = (unsigned int) *s2++;
		v = (unsigned int) c1 - (unsigned int) c2;
	} while (v == 0 && c1 != '\0');
	
	if (v < 0)
		return -1;
	else if (v > 0)
		return 1;
	else
		return v;
}

void compare(const char *s1, const char *s2) {
	int c = strcmp(s1, s2);
	printf("STDLIB Comparison between \'%s\' and \'%s\' is: %d\n", s1, s2, c);
	c = cmpStr(s1, s2);
	printf("MYFUNC Comparison between \'%s\' and \'%s\' is: %d\n", s1, s2, c);
}

void printInt(const void *ptr) {
	int *ptrVal = (int *)ptr;
	if (ptrVal)
		printf("%d", *ptrVal);
	else
		printf("NULL");
}

void printString(const void *ptr) {
	char *str = (char *)ptr;
	printf("%s", str);
}

int main(int argc, char *argv[]) {
	cds_hash_table *htable;
	cds_result cr;
	cr = cds_hash_table_create(&htable, 8, &cmpInt, &cmpStr, &naiveIntHash);
	if (cds_error_check(cr))
		return 1;
	float lf = cds_hash_table_load_factor(htable);
	
	/*
	cds_hash_node *testNode;
	int k = 5;
	int v = 7;
	cr = cds_hash_node_create(&testNode, &k, &v);
	int *vk = (int *) testNode->key;
	int *vv = (int *) testNode->value;
	printf("<k: %d v: %d>\n", *vk, *vv);
	*/
	/*
	char name[] = "Jeff";
	char longname[] = "Jeff Lansing";
	char empty[] = "";
	char name2[] = "Tim";
	char lname2[] = "McDerpisten";
	*/
	
	char * nameList[] = {"Jeff", "Jeff Lansing", "", "Tim", "McDerpisten"};
	int numNames = 5, i;
	for (i = 0; i < numNames; ++i) {
		int *tmp = (int *) cds_alloc(sizeof(int));
		*tmp = i;
		cr = cds_hash_table_add(htable, tmp, nameList[i]);
		if (cds_error_check(cr))
			return 1;
	}
	
	void *value;
	for (i = 0; i < numNames; ++i) {
		cr = cds_hash_table_get(htable, &i, &value);
		//printf("CR: %d\n", cr);
		if (cds_error_check(cr))
			return 1;
		cds_log("<k: ");
		cds_log("%d v: ", i);
		printString(value);
		printf(">\n");
	}
	
	int k = 0;
	cr = cds_hash_table_remove(htable, &k, CDS_DELETE_KEY);
	
	k = 2;
	cr = cds_hash_table_remove(htable, &k, CDS_DELETE_KEY);
	
	printf("\nPrinting the contents after removal...\n");
	for (i = 0; i < numNames; ++i) {
		cr = cds_hash_table_get(htable, &i, &value);
		//printf("CR: %d\n", cr);
		if (cds_error_check(cr))
			return 1;
		if (cr != CDS_NOT_FOUND) {
			printf("<k: ");
			printf("%d v: ", i);
			printString(value);
			printf(">\n");
		}
	}
	/*
	printf("Setting all values to:");
	for (i = 0; i < numNames; ++i) {
		//int *tmp = (int *) cds_alloc(sizeof(int));
		//*tmp = i;
		//void *vtmp = (void *) tmp;
		cr = cds_hash_table_get(htable, &i, &value);
		//printf("CR: %d\n", cr);
		if (cds_error_check(cr))
			return 1;
		printString(value);
		printf("\n");
	}
	*/
	
	/*
	cr = cds_hash_table_delete(&htable);
	if (cds_error_check(cr))
		return 1;
	*/
	/*
	compare(name, longname);
	compare(longname, name);
	compare(name, name);
	compare(longname, name);
	compare('\0', name);
	compare(name, empty);
	compare(empty, name);
	*/
	return 0;
}