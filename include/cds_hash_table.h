/**
 * @file cds_hash_table.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Hash Table
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CDS_HASH_TABLE_H_
#define _CDS_HASH_TABLE_H_

/** 
 * @addtogroup CDS_HASH_TABLE
 * @{
 */
#include "cds_hash_node.h"

/** Default reccomended start size */
#define CDS_DEFAULT_HASH_TABLE_SIZE 8

/** Default growth rate when a hash table reaches its capacity */
#define CDS_DEFAULT_HASH_TABLE_GROWTH_MULTIPLIER 2

/**
 * @struct cds_hash_table cds_hash_table.h
 * A struct container for hash table using linear probing.  Stores a 
 * dynamically resizable group of buckets, count/size, and function 
 * pointers for key comparisons, value comparisons, and hashing.
 */
struct cds_hash_table {
	cds_hash_node **buckets;
	unsigned int count;
	unsigned int size;
	cds_cmp_func key_cmp_func;
    cds_cmp_func value_cmp_func;
	cds_hash_func hash_func;
};

/** @typedef cds_hash_table type */
typedef struct cds_hash_table cds_hash_table;

/** Creates the table */
cds_result cds_hash_table_create(cds_hash_table **table, unsigned int size, cds_cmp_func key_cmp, cds_cmp_func val_cmp, cds_hash_func hash_func);

/** Deletes the table */
cds_result cds_hash_table_delete(cds_hash_table **table, cds_hash_node_deletion_behavoir db);

/** Clears the table */
cds_result cds_hash_table_clear(cds_hash_table *table, cds_hash_node_deletion_behavoir db);

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

/** Gets the load factor of the table (num total elements / table array size) */
float cds_hash_table_load_factor(const cds_hash_table *table);

/** Gets the data given the key and puts it into value */
cds_result cds_hash_table_get(const cds_hash_table *table, const void *key, void **value);

/** Sets the value of the hash table at the given key */
cds_result cds_hash_table_set(cds_hash_table *table, const void *key, const void* value);

/** Adds the given key/value pair to the table */
cds_result cds_hash_table_add(cds_hash_table *table, const void *key, const void *value);

/** Removes the entry with the given key */
cds_result cds_hash_table_remove(cds_hash_table *table, const void *key, cds_hash_node_deletion_behavoir db);

/** Iterates through every key in the collection */
cds_result cds_hash_table_iterate_keys(const cds_hash_table *table, cds_visit_func visit_func);

/** Iterates through every value in the collection */
cds_result cds_hash_table_iterate_values(const cds_hash_table *table, cds_visit_func visit_func);

/** Iterates through every key/value pair in the collection */
cds_result cds_hash_table_iterate(const cds_hash_table *table, cds_visit_pair_func visit_pair_func);

/** @} */
#endif

#ifdef __cplusplus
}
#endif
