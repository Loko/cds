#include "cds_dynamic_array.h"

unsigned int arrayDoubleGrowth(unsigned int oldSize) {
	if (oldSize) {
		return oldSize * 2;
	} else {
		return CDS_DEFAULT_DYNAMIC_ARRAY_SIZE;
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
	int *pi = (int *) ptr;
	cds_log("%d", *pi);
} 

void printString(const void *ptr) {
	char *ps = (char *) ptr;
	cds_log("%s", ps);
}

void printString2(const void *ptr) {
	char *ps = (char *) ptr;
	cds_log("%s\n", ps);
}

void logArray(const cds_dynamic_array *array, int printSize) {
	if (array) {
		if (printSize)
			cds_log("Count: %d\nSize: %d\n", array->count, array->size);
		unsigned int count = cds_dynamic_array_count(array);
		unsigned int i;
		for (i = 0; i < count; ++i) {
			cds_log("%u: %s\n", i, (char *)array->buffer[i]);
		}
	}
}

int main(void) {
	/* create the array */
	cds_dynamic_array *array = NULL;
	unsigned int size = CDS_DEFAULT_DYNAMIC_ARRAY_SIZE; /* 4 by default */
	cds_result cr = cds_dynamic_array_create(&array, size); /* notice the address of the pointer is passed in */
    
	/* checks for an error, prints and returns 1 if something failed
 	 * we do this (in this example) repeatedly for safety
	 */
	if (cds_error_check(cr)) return 1;
	
	/* add the strings to the container */
	char * presidents[] = {"washington", "jefferson", "roosevelt", "reagan", "clinton", "obama"};
	unsigned int numPresidents = 6;
	unsigned int i;
	for (i = 0; i < numPresidents; ++i) {
		cr = cds_dynamic_array_push_back(array, presidents[i]); /* will need to reallocate when i == 4 */
		if (cds_error_check(cr)) return 1;
	}
	
	/* print out the array */
	cds_log("Displaying the contents of the dynamic array...\n");
	logArray(array, 1);
	
	/* try adding past the current size of the array */
	void *curValue;
	cds_log("Adding some more strings to the container...\n");
	char *otherPresidents[] = {"adams", "bush", "coolidge", "kennedy"};
	unsigned int numOtherPresidents = 4;
	/* since the default size is 4, we've grown once, 4 * 1.5 = 6 */
	/* therefore, on the next insertion, the array will need to grow */
	/* we can use a custom growth function, to resize things differently */
	for (i = 0; i < numOtherPresidents; ++i) {
		cr = cds_dynamic_array_push_back_gf(array, otherPresidents[i], &arrayDoubleGrowth);
		if (cds_error_check(cr))
			return 1;
	}
	
	/* lets try printing the array using an iterate function */
	cds_log("Printing the contents of the array using the iterate function...\n");
	cr = cds_dynamic_array_iterate(array, &printString2);
	if (cds_error_check(cr)) return 1;
	
	/* accessing the last element first just for the sake of printing it */
	unsigned int count = cds_dynamic_array_count(array);
	cr = cds_dynamic_array_get(array, count - 1, &curValue);
	cds_log("Removing the last element in the collection: %s", (char *)curValue);
	
	/* remove the element at the end of the array */
	cr = cds_dynamic_array_pop_back(array);
	if (cds_error_check(cr)) return 1;
	
	/* this will remove the element from the container, but give it as a pointer */
	cr = cds_dynamic_array_pop_back_data(array, &curValue);
	if (cds_error_check(cr)) return 1;
	cds_log("Removed: %s from the back of the array\n", (char *) curValue);
	
	/* remove functions will rely on pointer addresses by default
	 * we know that roosevelt's address is in the array so this will work */
	cr = cds_dynamic_array_remove(array, presidents[2]);
	if (cds_error_check(cr)) return 1;
	
	/* if we want to remove with value equality, we must use the cmp functions 
	 * so while "adams" is in the array, this is a new address
	 */
	char adamsStr[] = "adams";
	cr = cds_dynamic_array_remove(array, adamsStr);
	char resultString[CDS_RESULT_MAX_STR_LEN];
	cds_result_string(cr, resultString);
	cds_log("Attempt to remove the same string at a different address: %s\n", resultString);
	
	/* if we use our strcmp equivalent, this can be done with the proper function */
	cr = cds_dynamic_array_remove_cmp(array, adamsStr, &cmpStr);
	cds_result_string(cr, resultString);
	cds_log("Attempt to remove the same string using the comparison function: %s\n", resultString);
	
	logArray(array, 0);
	
	/* you can also remove elements by index (with safe bounds checking) */
	cds_log("Removing from index 2...\n");
	cr = cds_dynamic_array_remove_at(array, 2);
	logArray(array, 0);
	
	/* The dynamic array supports multiple removal behavoirs
	 * By default it is shift down, which maintains the relative order of elements
	 * if remove_at(2) CDS_SHIFT DOWN: a b c d e f becomes a b d e f
	 * It also supports replacing the removed element with the element at the end
	 * This disrupts the relative order of elements, but is more efficient than shifting large arrays
	 * if remove_at(2) CDS_REPLACE_WITH_LAST a b c d e f becomes a b f d e
	 */
	cds_log("Removing from index 2 using CDS_REPLACE_WITH_LAST\n");
	cr = cds_dynamic_array_remove_at_rb(array, 2, CDS_REPLACE_WITH_LAST);
	logArray(array, 0);
	
	/* Reverse the relative order of elements */
	cds_log("Reversing the order of elements in the dynamic array...\n");
	cr = cds_dynamic_array_reverse(array);
	logArray(array, 0);
	
	cds_log("Deleting the dynamic array...\n");
	cr = cds_dynamic_array_delete(&array);
	if (cds_error_check(cr))
		return 1;
	cds_log("Deletion successful...\n");
	return 0;
}
