/**
 * @file cds_dlist.h
 * @author Jeff Lansing
 * @brief A Collection of Functions For A Doubly Linked List
 */

#ifndef _CDS_DLIST_H_
#define _CDS_DLIST_H_

/** 
 * @addtogroup CDS_DLIST
 * @{
 */


#ifndef CDS_DLIST_FORCE_NODE_SEARCH
/** 
 * Determines if a linear search will be performed when removing a node from a list 
 * A more expensive, but safer check will be executed if this is set to 1 (default is 0)
 */
#define CDS_DLIST_FORCE_NODE_SEARCH 0
#endif

#include "cds_core.h"
#include "cds_dlnode.h"

/**
 * A struct for a double linked list of any type
 */
struct cds_dlist {
	cds_dlnode *head;
	cds_dlnode *tail;
	unsigned int count;
	/** @todo could potentially put a pointer to the container itself here (added safety) */
};

/**  */
typedef struct cds_dlist cds_dlist;

/** Creates the doubly linked list  */
cds_result cds_dlist_create(cds_dlist **list);

/** Deletes the doubly linked list */
cds_result cds_dlist_delete(cds_dlist **list);

/** Deletes the nodes from the list but not the data or the list itself */
cds_result cds_dlist_clear(cds_dlist *list);

/** Deletes the nodes from the list, the data pointers in the nodes, and the list itself  */
cds_result cds_dlist_delete_all(cds_dlist **list);

/** Gets the number of nodes in the list */
unsigned int cds_dlist_count(const cds_dlist *list);

/** Gets the head node of the list */
cds_dlnode * cds_dlist_head(const cds_dlist *list);

/** Gets the tail node of the list */
cds_dlnode * cds_dlist_tail(const cds_dlist *list);

/** Adds a new node with the given data to the head of the list */
cds_result cds_dlist_add_first(cds_dlist *list, const void *data);

/** Adds a new node with the given data to the tail of the list */
cds_result cds_dlist_add_last(cds_dlist *list, const void *data);

/** Inserts a new node with the given data before the passed in node */
cds_result cds_dlist_insert_before(cds_dlist *list, cds_dlnode *node, const void *data);

/** Inserts a new node with the given data after the passed in node */
cds_result cds_dlist_insert_after(cds_dlist *list, cds_dlnode *node, const void *data);

/** Removes the head node from the list */
cds_result cds_dlist_remove_head(cds_dlist *list);

/** Removes the tail node from the list */
cds_result cds_dlist_remove_tail(cds_dlist *list);

/** Removes the head node from the list and puts its data pointer into data */
cds_result cds_dlist_remove_head_data(cds_dlist *list, void **data);

/** Removes the tail node from the list and puts its data pointer into data */
cds_result cds_dlist_remove_tail_data(cds_dlist *list, void **data);

/** Removes the node with the given data pointer */
cds_result cds_dlist_remove(cds_dlist *list, const void *data);

/** Removes the node from the list */
cds_result cds_dlist_remove_node(cds_dlist *list, cds_dlnode *node);

/** Removes the node from the list with the given  */
cds_result cds_dlist_remove_cmp(cds_dlist *list, const void *data, cds_cmp_func cmp_func);

/** Finds the node in the list with the given data address */
cds_result cds_dlist_find(const cds_dlist *list, const void *data, cds_dlnode **node);

/** Finds the node in the list with the given value */
cds_result cds_dlist_find_cmp(const cds_dlist *list, const void *data, cds_dlnode **node, cds_cmp_func cmp_func);

/** Safely iterates and visits every element in the list */
cds_result cds_dlist_iterate(const cds_dlist *list, cds_visit_func visit_func);

/** @todo: reverse function here for doubly linked list */
cds_result cds_dlist_reverse(cds_dlist *list);

/** @} */
#endif
