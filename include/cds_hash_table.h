#ifndef _CDS_HASH_TABLE_H_
#define _CDS_HASH_TABLE_H_

#include "cds_hash_node.h"

struct cds_hash_table {
	cds_hash_node **buckets;
	unsigned int count;
	unsigned int size;
	cds_cmp_func cmp_func;
	cds_hash_func hash_func;
};
typedef struct cds_hash_table cds_hash_table;

cds_result cds_hash_table_create(cds_hash_table **table, unsigned int size, cds_cmp_func cmp_func, cds_hash_func hash_func);
cds_result cds_hash_table_delete(cds_hash_table **table);
cds_result cds_hash_table_clear(cds_hash_table *table);
cds_result cds_hash_table_delete_all(cds_hash_table **table);

unsigned int cds_hash_table_count(cds_hash_table *table);
unsigned int cds_hash_table_size(cds_hash_table *table);
// cmp_func
// hash_func


cds_result cds_hash_table_add(cds_hash_table *table, void *key, void *data);
cds_result cds_hash_table_remove(cds_hash_table *table, void *key);
cds_result cds_hash_table_find(cds_hash_table *table, cds_hash_table **node);

cds_result cds_hash_table_iterate(cds_hash_table *table, cds_visit_func visit_func);

#endif