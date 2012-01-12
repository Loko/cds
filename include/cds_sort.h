/**
 * @file cds_sort.h
 * @author Jeff Lansing
 * Supports sorting operations
 */
 
#ifndef _CDS_SORT_H_
#define _CDS_SORT_H_
 
#include "cds_core.h"

/**  */
cds_result cds_array_shell_sort(void *buffer, unsigned int n, cds_cmp_func cmp_func);
 
 
#endif