/**
 * @file cds_hash_table.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Hash Table
 */

#ifndef _CDS_HASH_TABLE_H_
#define _CDS_HASH_TABLE_H_

/** 
 * @addtogroup CDS_HASH_TABLE
 * @{
 */
#include "cds_hash_node.h"

 /**
  *
  */
struct cds_hash_table {
	cds_hash_node **buckets;
	unsigned int count;
	unsigned int size;
	cds_cmp_func key_cmp_func;
    cds_cmp_func value_cmp_func;
	cds_hash_func hash_func;
};

/** */
typedef struct cds_hash_table cds_hash_table;

/** Creates the table */
cds_result cds_hash_table_create(cds_hash_table **table, unsigned int size, cds_cmp_func key_cmp, cds_cmp_func val_cmp, cds_hash_func hash_func);

/** Deletes the table */
cds_result cds_hash_table_delete(cds_hash_table **table);

/** Clears the table */
cds_result cds_hash_table_clear(cds_hash_table *table);

/** Deletes the table, the keys, and the values */
cds_result cds_hash_table_delete_all(cds_hash_table **table);

/** Gets the number of elements in the table */
unsigned int cds_hash_table_count(const cds_hash_table *table);

/** Gets the size of the table (uses chaining) */
unsigned int cds_hash_table_size(const cds_hash_table *table);

/** Gets the key comparison function used by the table */
cds_cmp_func cds_hash_table_key_cmp_func(const cds_hash_table *table);

/** Gets the value comparison function used by the table */
cds_cmp_func cds_hash_table_value_cmp_func(const cds_hash_table *table);

/** Gets the data given the key and puts it into value */
cds_result cds_hash_table_get(const cds_hash_table *table, const void *key, void **value);

/** Sets the value of the hash table at the given key */
cds_result cds_hash_table_set(cds_hash_table *table, const void *key, const void* value);

/**  */
cds_result cds_hash_table_add(cds_hash_table *table, const void *key, const void *data);

/** */
cds_result cds_hash_table_remove(cds_hash_table *table, const void *key);

/** */
cds_result cds_hash_table_find(const cds_hash_table *table, cds_hash_node **node);

/** */
cds_result cds_hash_table_iterate(const cds_hash_table *table, cds_visit_func visit_func);

/** */
cds_result cds_hash_table_iterate_nodes(cds_hash_table *table, cds_visit_func visit_func);

/** @} */
#endif
