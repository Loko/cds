#include "cds_dynamic_array.h"



int test_func(cds_dynamic_array *a) {
	if (a) {
		a->size = 100;
		return 1;
	} else {
		return 0;
	}
}

// assume it is an int for now
void test_visit_func(const void *ptr) {
	char *pC = (char *) ptr;
	printf("%c\n", *pC);
}

void test_visit_func2(const void *ptr) {
    int *pI = (int *) ptr;
    printf("%d\n", *pI);
}

int main(void) {
	printf("before assertion\n");
	CDS_ASSERT_MSG(0 == 1, "Jeff Is: %s", "Tired");
	printf("after assertion\n");
	
	cds_dynamic_array *array = NULL;
	cds_result cr = cds_dynamic_array_create(&array, 8);
	//cds_result cr = cds_dynamic_array_create(array, 8);
    
	printf("%d\n", cr);
	if (!cds_is_error(cr)) {
		printf("Size = %u\n", array->size);
		printf("Count = %u\n", array->count);
		int i;
		char values[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
		//printf("First Value: %c\n", values[1]);
		//int *tmpAlloc;
		for (i = 0; i < 8; ++i) {
			//tmpAlloc = malloc(sizeof(int));
			//*tmpAlloc = i;
            
			cr = cds_dynamic_array_push_back(array, values + i);
			printf("%c\n", *((char *)array->buffer[i]));
		}
		
		void *tmp;
		char *c;
		for (i = 0; i < 8; ++i) {
			cds_result r = cds_dynamic_array_get(array, i, &tmp);
			//cds_result r = cds_dynamic_array_get2(&array, i, &tmp);
			printf("%d\n", r);
			if (r == CDS_OK) {
				c = (char *)tmp;
				printf("%c\n", *c);
			}
		}
		
		printf("Size: %u\n", array->size);
		test_func(array);
		printf("Size: %u\n", array->size);
		
		printf("Printing at end...\n");
		for (i = 0; i < array->count; ++i) {
			printf("%u: %c\n", i, (*(char *)array->buffer[i]));
		}
		
		cds_dynamic_array_set(array, 0, values + 3);
		printf("Printing again...\n");
		for (i = 0; i < array->count; ++i) {
			printf("%u: %c\n", i, (*(char *)array->buffer[i]));
		}
		
		cr = cds_dynamic_array_resize(&array, 6);
		printf("resize result: %d\n", cr);
		cds_dynamic_array_delete(&array);
		if (array) {
			printf("still has a value...\n");
		}
		
		cr = cds_dynamic_array_create(&array, 20);
		printf("cr: %d\n", cr);
        const int ia = 1;
        const int ib = 2;
        const int ic = 3;
        const int id = 4;
		cds_dynamic_array_push_back(array, &ia);
		cds_dynamic_array_push_back(array, &ib);
		cds_dynamic_array_push_back(array, &ic);
		cds_dynamic_array_push_back(array, &id);
		for (i = 0; i < array->count; ++i) {
			printf("%u: %i\n", i, (*(int *)array->buffer[i]));
		}
		
		void *t;
        
		cr = cds_dynamic_array_remove_rb(array, &ic, CDS_REPLACE_WITH_LAST);
		if (!cds_is_error(cr)) {
			char error[CDS_MAX_ERR_STR_LEN];
			cds_result_string(cr, error);
			printf("%s\n", error);
		}
		
		unsigned int idx;
		if (cds_dynamic_array_find(array, &id, &idx) == CDS_OK) {
			printf("Index Of: %d\n", idx);
		}
		
		//cds_dynamic_array_pop_back_data(array, &t);
		//cds_dynamic_array_remove_at(array, 0);
		//cds_dynamic_array_remove_at_rb(array, 1, CDS_REPLACE_WITH_LAST);
		//printf("%c\n", (*(char *)t));
		//cds_dynamic_array_pop_back_data(array, &t);
		//printf("%c\n", (*(char *)t));
		//for (i = 0; i < array->count; ++i) {
		//	printf("%u: %c\n", i, (*(char *)array->buffer[i]));
		//}
		

		printf("%s", "Trying this with the iterate function:\n");
		cr = cds_dynamic_array_iterate(array, &test_visit_func2);
		if (cds_error_check(cr))
			return 1;
	}
	return 0;
}