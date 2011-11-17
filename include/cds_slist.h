#ifndef _CDS_SLIST_H_
#define _CDS_SLIST_H_

#include "cds_slnode.h"

struct cds_slist {
	cds_slnode *head;
	cds_slnode *tail;
	unsigned int count;
};
typedef struct cds_slist cds_slist;

cds_result cds_slist_create(cds_slist **list);
cds_result cds_slist_delete(cds_slist **list);
cds_result cds_slist_clear(cds_slist *list);
cds_result cds_slist_delete_all(cds_slist **list);

unsigned int cds_slist_count(const cds_slist *list);
cds_slnode * cds_slist_head(const cds_slist *list);
cds_slnode * cds_slist_tail(const cds_slist *list);

cds_result cds_slist_add_first(cds_slist *list, const void *data);
cds_result cds_slist_add_last(cds_slist *list, const void *data);
cds_result cds_slist_insert_before(cds_slist *list, cds_slnode *node, const void *data);
cds_result cds_slist_insert_after(cds_slist *list, cds_slnode *node, const void *data);

cds_result cds_slist_remove_head(cds_slist *list);
cds_result cds_slist_remove_tail(cds_slist *list);
cds_result cds_slist_remove_head_data(cds_slist *list, void **data);
cds_result cds_slist_remove_tail_data(cds_slist *list, void **data);

cds_result cds_slist_remove(cds_slist *list, const void *data);
cds_result cds_slist_remove_node(cds_slist *list, cds_slnode *node);
cds_result cds_slist_remove_cmp(cds_slist *list, const void *data, const cds_cmp_func cmp_func);

cds_result cds_slist_find(const cds_slist *list, const void *data, cds_slnode **node);
cds_result cds_slist_find_cmp(const cds_slist *list, const void *data, cds_slnode **node, const cds_cmp_func cmp_func);

cds_result cds_slist_iterate(const cds_slist *list, const cds_visit_func visit_func);
cds_result cds_slist_reverse(cds_slist *list);
//cds_result 
#endif