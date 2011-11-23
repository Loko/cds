#include "cds_hash_table.h"
#include "stdio.h"
#include "string.h"

unsigned int naiveIntHash(const void *key) {
	int *ikey = (int *) key;
	*ikey = (*ikey) < 0 ? -(*ikey) : *ikey;
	return (unsigned int)(*ikey);
}

/* http://en.wikipedia.org/wiki/Jenkins_hash_function */
unsigned int jenkinsHash(const void *key) {
	
	char *str = (char *) key;
	char *c = str;
	unsigned int len = 0;
	while (*c++)
		++len;
	
	unsigned int hash, i;
	for(hash = i = 0; i < len; ++i) {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
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

void printInt(const void *ptr) {
	int *ptrVal = (int *)ptr;
	if (ptrVal)
		cds_log("%d", *ptrVal);
	else
		cds_log("NULL");
}

void printString(const void *ptr) {
	char *ptrStr = (char *)ptr;
	if (ptrStr) {
		cds_log("%s", ptrStr);
	} else {
		cds_log("NULL");
	}

}

void printPair(const void *keyPtr, const void *valPtr) {
	cds_log("<k: ");
	printInt(keyPtr);
	cds_log(" v: ");
	printString(valPtr);
	cds_log(">\n");
}

void logTable(const cds_hash_table *htable, int printSize) {
	if (htable) {
		if (printSize) {
			cds_log("Count: %u\nSize: %u\nLoad: %f\n", cds_hash_table_count(htable), cds_hash_table_size(htable), cds_hash_table_load_factor(htable));
		}
		cds_hash_table_iterate(htable, &printPair);
	}
}

int main(int argc, char *argv[]) {
	/* Create the hash table */
	cds_hash_table *htable = NULL;
	cds_result cr;
	cr = cds_hash_table_create(&htable, 8, &cmpInt, &cmpStr, &naiveIntHash);
	if (cds_error_check(cr)) return 1;
	
	/* Add pairs of keys: ints and values: strings */
	cds_log("Adding keys as ints, strings as values...\n");
	int keys[] = {0, 1, 2, 3, 4};
	char * namesList[] = {"Jeff", "Jeff Lansing", "Hughes", "Tim", "McDerpisten"};
	int numNames = 5, i;
	for (i = 0; i < numNames; ++i) {
		cr = cds_hash_table_add(htable, keys + i, namesList[i]);
		if (cds_error_check(cr)) return 1;
	}
	
	logTable(htable, 1);
	
	/* Demonstrate using get/set functions */
	cds_log("Trying to put in and take out data...\n");
	char newName[] = "Mario";
	void *oldName = NULL;
	cr = cds_hash_table_get(htable, keys  + 2, &oldName);
	if (cds_error_check(cr)) return 1;
	cds_log("Old name at Key: %d is: %s\n", keys[2], (char *)oldName);
	cds_log("Trying to set it to: %s\n", newName);
	void *name = NULL;
	cr = cds_hash_table_set(htable, keys + 2, newName);
	if (cds_error_check(cr)) return 1;
	cr = cds_hash_table_get(htable, keys + 2, &name);
	if (cds_error_check(cr)) return 1;
	cds_log("The newly set name at that key is: %s\n", (char *) name);
	cds_log("Confirming Equality (strcmp): %d\n", cmpStr(newName, (char *) name));
	
	/* remove some values by key into the table
	 * if the key or value was dynamically allocated, you would need to pass in 
	 * CDS_DELETE_KEY or CDS_DELETE_VALUE or CDS_DELETE_ALL
	 * These can be OR'd together if you want
	 * In this instance, the referenced values are garbage at the end of the func anyways so it's not necessary
	 */
	cds_log("Attempting to remove some entries from the table...\n");
	cr = cds_hash_table_remove(htable, &keys[1], CDS_DELETE_NODE_ONLY);
	if (cds_error_check(cr)) return 1;
	cr = cds_hash_table_remove(htable, &keys[4], CDS_DELETE_NODE_ONLY);
	if (cds_error_check(cr)) return 1;
	
	logTable(htable, 1);
	
	/* delete the hash table */
	cr = cds_hash_table_delete(&htable, CDS_DELETE_NODE_ONLY);
	if (cds_error_check(cr)) return 1;
	
	return 0;
}