/**
 * @file cds_slist.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Singly Linked List
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CDS_SLIST_H_
#define _CDS_SLIST_H_

/**
 * @addtogroup CDS_SLIST
 * @{
 */

#include "cds_core.h"
#include "cds_slnode.h"
 
/**
 * Singly Linked List stores the head, tail, and count
 */
struct cds_slist {
	cds_slnode *head;
	cds_slnode *tail;
	unsigned int count;
};

/** @typedef cds_slist type */
typedef struct cds_slist cds_slist;

/** Creates the singly linked list */
cds_result cds_slist_create(cds_slist **list);

/** Deletes the singly linked list */
cds_result cds_slist_delete(cds_slist **list);

/** Clears the singly linked list, deleting all of its nodes, but doesn't delete the data pointers */
cds_result cds_slist_clear(cds_slist *list);

/** Deletes the singly linked list and the data pointers */
cds_result cds_slist_delete_all(cds_slist **list);

/** Gets the number of elements in the singly linked list */
unsigned int cds_slist_count(const cds_slist *list);

/** Gets the head node of the singly linked list */
cds_slnode * cds_slist_head(const cds_slist *list);

/** Gets the tail node of the singly linked list */
cds_slnode * cds_slist_tail(const cds_slist *list);

/** Adds the given data pointer to the head of the list */
cds_result cds_slist_add_first(cds_slist *list, const void *data);

/** Adds the given data pointer to the end of the list */
cds_result cds_slist_add_last(cds_slist *list, const void *data);

/** Inserts the data in a new node before the passed in node */
cds_result cds_slist_insert_before(cds_slist *list, cds_slnode *node, const void *data);

/** Inserts the data in a new node before the passed in node and puts the newly created node into cnode */
cds_result cds_slist_insert_before_node(cds_slist *list, cds_slnode *node, const void *data, cds_slnode **cnode);

/** Inserts the data in a new node after the passed in node */
cds_result cds_slist_insert_after(cds_slist *list, cds_slnode *node, const void *data);

/** Inserts after the given node and puts the newly created node into cnode */
cds_result cds_slist_insert_after_node(cds_slist *list, cds_slnode *node, const void *data, cds_slnode **cnode);

/** Removes the head node from the list */
cds_result cds_slist_remove_head(cds_slist *list);

/** Removes the tail node from the list */
cds_result cds_slist_remove_tail(cds_slist *list);

/** Removes the head node from the list and puts its pointer in data  */
cds_result cds_slist_remove_head_data(cds_slist *list, void **data);

/** Removes the tail node from the list and puts its pointer in data */
cds_result cds_slist_remove_tail_data(cds_slist *list, void **data);

/** Removes the node from the list with given data address pointer */
cds_result cds_slist_remove(cds_slist *list, const void *data);

/** Removes the given node from the list */
cds_result cds_slist_remove_node(cds_slist *list, cds_slnode *node);

/** Removes the node with the given data from the list using the comparison function.  Only the first node with the given value will be removed.  */
cds_result cds_slist_remove_cmp(cds_slist *list, const void *data, cds_cmp_func cmp_func);

/** Searches for the given data address in the list.  If found, puts the node into the node pointer. */
cds_result cds_slist_find(const cds_slist *list, const void *data, cds_slnode **node);

/** Searches for the given data value in the list.  If found, puts the node into the node pointer. */
cds_result cds_slist_find_cmp(const cds_slist *list, const void *data, cds_slnode **node, cds_cmp_func cmp_func);

/** Iteratively reverses the singly linked list */
cds_result cds_slist_reverse(cds_slist *list);

/** Iterates through the list and accesses every element using the visit function */
cds_result cds_slist_iterate(const cds_slist *list, cds_visit_func visit_func);

/** @} */
#endif

#ifdef __cplusplus
}
#endif 
