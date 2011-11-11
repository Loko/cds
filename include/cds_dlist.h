#ifndef _CDS_DLIST_H_
#define _CDS_DLIST_H_

// if set to 1, the remove node will confirm the node exists in the list (note this is expensive)
#define CDS_DLIST_SEARCH_FOR_RNODE 0

#include "cds_core.h"
#include "cds_dlnode.h"

struct cds_dlist {
	cds_dlnode *head;
	cds_dlnode *tail;
	unsigned int count;
	// could potentially put a pointer to the container itself here (added safety)
};
typedef struct cds_dlist cds_dlist;

cds_result cds_dlist_create(cds_dlist **list);
cds_result cds_dlist_delete(cds_dlist **list);
cds_result cds_dlist_clear(cds_dlist *list);
cds_result cds_dlist_delete_all(cds_dlist **list);

unsigned int cds_dlist_count(cds_dlist *list);
cds_dlnode * cds_dlist_head(cds_dlist *list);
cds_dlnode * cds_dlist_tail(cds_dlist *list);

cds_result cds_dlist_add_first(cds_dlist *list, void *data);
cds_result cds_dlist_add_last(cds_dlist *list, void *data);
// have an add_first_node and add_last_node function that lets the higher level keep the node
cds_result cds_dlist_insert_before(cds_dlist *list, cds_dlnode *node, void *data);
cds_result cds_dlist_insert_after(cds_dlist *list, cds_dlnode *node, void *data);

cds_result cds_dlist_remove_head(cds_dlist *list);
cds_result cds_dlist_remove_tail(cds_dlist *list);
cds_result cds_dlist_remove_head_data(cds_dlist *list, void **data);
cds_result cds_dlist_remove_tail_data(cds_dlist *list, void **data);

cds_result cds_dlist_remove(cds_dlist *list, void *data);
cds_result cds_dlist_remove_node(cds_dlist *list, cds_dlnode *node);
// should have a _rb node func to search or not
cds_result cds_dlist_remove_cmp(cds_dlist *list, void *data, cds_cmp_func cmp_func);

cds_result cds_dlist_find(cds_dlist *list, void *data, cds_dlnode **node);
cds_result cds_dlist_find_cmp(cds_dlist *list, void *data, cds_dlnode **node, cds_cmp_func cmp_func);

cds_result cds_dlist_iterate(cds_dlist *list, cds_visit_func visit_func);
#endif

