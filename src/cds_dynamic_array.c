#include "cds_dynamic_array.h"

/* allocates the dynamic array to the given size
 * a size of zero is valid, but the buffer will not be allocated
 * Caution, most functions here will assume buffer has been allocated!!
 */
cds_result cds_dynamic_array_create(cds_dynamic_array **array, unsigned int size) {
	*array = (cds_dynamic_array *) cds_alloc(sizeof(cds_dynamic_array));
	if (*array) {
		if (size > 0) {
			(*array)->buffer = (void *) cds_alloc(size * sizeof(void *));
			if ((*array)->buffer) {
				(*array)->size = size;
			} else {
				cds_free(*array);
				*array = NULL;
				return CDS_BAD_ALLOC;
			}
		} else {
			(*array)->buffer = NULL;
			(*array)->size = 0;
		}
		(*array)->count = 0;
		return CDS_OK;
	} else {
		return CDS_BAD_ALLOC;
	}
}

/* frees the array structure and buffer
 * item pointers in the array will NOT be released
 */
cds_result cds_dynamic_array_delete(cds_dynamic_array **array) {
	if (*array) {
		cds_free((*array)->buffer);
		cds_free(*array);
		*array = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dynamic_array_delete_all(cds_dynamic_array **array) {
	if (*array) {
		/* free all the pointers first */
		/** @todo Iterate until size or until count? */
		unsigned int i;
		for (i = 0; i < (*array)->size; ++i) {
			cds_free((*array)->buffer[i]);
			(*array)->buffer[i] = NULL;
		}
		cds_free((*array)->buffer);
		cds_free(*array);
		*array = NULL;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* resizes the dynamic array to the given size */
cds_result cds_dynamic_array_resize(cds_dynamic_array **array, unsigned int size) {
	if (*array) {
		(*array)->buffer = (void *) cds_realloc((*array)->buffer, size);
		if ((*array)->buffer) {
			(*array)->size = size;
			if ((*array)->size < (*array)->count) {
				/* do we need to free the excess pointers?  just in the buffer? */
				(*array)->count = (*array)->size;
			}
			return CDS_OK;
		} else {
			(*array)->size = 0;
			(*array)->count = 0;
			return (size) ? CDS_BAD_ALLOC : CDS_OK;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* size does not change, we don't free stuff here */
cds_result cds_dynamic_array_clear(cds_dynamic_array *array) {
	if (array) {
		array->count = 0;
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
unsigned int cds_dynamic_array_count(const cds_dynamic_array *array) {
	unsigned int count = 0;
	if (array) {
		count = array->count;
	}
	return count;
}

/* */
unsigned int cds_dynamic_array_size(const cds_dynamic_array *array) {
	unsigned int size = 0;
	if (array) {
		size = array->size;
	}
	return size;
}

/* safely gets a pointer from the array at the given index
 * the data pointer is valid if CDS_OK is returned
 */
cds_result cds_dynamic_array_get(const cds_dynamic_array *array, unsigned int index, void **data) {
	if (array && *data) {
		if (index < array->count) {
			*data = array->buffer[index];
			return CDS_OK;
		} else {
			return CDS_INDEX_OUT_OF_RANGE;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* sets the address at the given index to data
 * the data pointer has been successfully set if CDS_OK is returned
 */
cds_result cds_dynamic_array_set(cds_dynamic_array *array, unsigned int index, const void *data) {
	if (array && data) {
		if (index < array->count) {
			array->buffer[index] = (void *) data;
			return CDS_OK;
		} else {
			return CDS_INDEX_OUT_OF_RANGE;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* adds the pointer to the end of the array */
cds_result cds_dynamic_array_push_back(cds_dynamic_array *array, const void *data) {
	if (array && data) {
		cds_result cr = CDS_OK;
		/* resize if we are at capacity */
		if (array->count == array->size) {
			unsigned int nsize = (array->buffer) ? array->size * CDS_DEFAULT_DYNAMIC_ARRAY_GROWTH_MULTIPLIER : CDS_DEFAULT_DYNAMIC_ARRAY_SIZE;
			void *tmp = (void *) cds_realloc(array->buffer, nsize * sizeof(void *));
			if (tmp) {
				array->buffer = tmp;
				array->size = nsize;
			} else {
				cr = CDS_BAD_ALLOC;
			}
		}
		/* if nothing went wrong with resizing, place at the end of the buffer */
		/* we carry the cds_result to avoid annoying warning saying there is no return */
		if (cr == CDS_OK) {
			array->buffer[array->count] = (void *)data;
			array->count++;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/*
 * adds the pointer to the end of the array
 * custom growth function can determine the new size of the array
 * if it is null, the default type of reallocation is used
 */
cds_result cds_dynamic_array_push_back_gf(cds_dynamic_array *array, const void *data, cds_growth_func growth_func) {
	if (array && data) {
		cds_result cr = CDS_OK;
		/* handle resizing and allocations if necessary */
		if (array->count == array->size) {
			/* find the new size */
			unsigned int nsize;
			if (growth_func) {
				nsize = (*growth_func)(array->size);
				if (nsize <= array->size)
					return CDS_INVALID_OPERATION;
			} else {
				nsize = (array->buffer) ? array->size * CDS_DEFAULT_DYNAMIC_ARRAY_GROWTH_MULTIPLIER : CDS_DEFAULT_DYNAMIC_ARRAY_SIZE;
			}
			/* reallocate */
			void *tmp = (void *) cds_realloc(array->buffer, nsize * sizeof(void *));
			if (tmp) {
				array->buffer = tmp;
				array->size = nsize;
			} else {
				cr = CDS_BAD_ALLOC;
			}
		}
		/* add to the end of the buffer */
		if (cr == CDS_OK) {
			array->buffer[array->count] = (void *) data;
			array->count++;
		}
		return cr;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* removes the last element from the array */
cds_result cds_dynamic_array_pop_back(cds_dynamic_array *array) {
	if (array) {
		if (array->count) {
			array->count--;
			return CDS_OK;
		} else {
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* removes the last element from the array
 * address of removed item set to data pointer
 */
cds_result cds_dynamic_array_pop_back_data(cds_dynamic_array *array, void **data) {
	if (array) {
		if (array->count > 0) {
			*data = array->buffer[array->count - 1];
			array->count--;
			return CDS_OK;
		} else {
			*data = NULL;
			return CDS_UNDERFLOW;
		}
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* removes the element from the array using the default removal behavoir
 */
cds_result cds_dynamic_array_remove(cds_dynamic_array *array, const void *data) {
	return cds_dynamic_array_remove_rb(array, data, CDS_DEFAULT_REMOVAL_BEHAVOIR);
}

/* removes an element from the array using the given behavoir */
cds_result cds_dynamic_array_remove_rb(cds_dynamic_array *array, const void *data, cds_removal_behavoir rb) {
	if (array && data) {
		/*
		if (rb != CDS_SHIFT_DOWN && rb != CDS_REPLACE_WITH_LAST)
			return CDS_INVALID_ARGUMENT;
		*/
		unsigned int i;
		for (i = 0; i < array->count; ++i) {
			if (array->buffer[i] == data) {
				if (rb == CDS_SHIFT_DOWN) {
					while (i < array->count - 1) {
						array->buffer[i] = array->buffer[i + 1];
						++i;
					}
				} else if (rb == CDS_REPLACE_WITH_LAST) {
					array->buffer[i] = array->buffer[array->count - 1];
				} else {
					return CDS_INVALID_ARGUMENT;
				}
				array->count--;
				return CDS_OK;
			}
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dynamic_array_remove_cmp(cds_dynamic_array *array, const void *data, cds_cmp_func cmp_func) {
	return cds_dynamic_array_remove_cmp_rb(array, data, cmp_func, CDS_DEFAULT_REMOVAL_BEHAVOIR);
}

/* */
cds_result cds_dynamic_array_remove_cmp_rb(cds_dynamic_array *array, const void *data, cds_cmp_func cmp_func, cds_removal_behavoir rb) {
	if (array && data && cmp_func) {
		unsigned int i;
		for (i = 0; i < array->count; ++i) {
			if ((*cmp_func)(array->buffer[i], data) == 0) {
				if (rb == CDS_SHIFT_DOWN) {
					while (i < array->count - 1) {
						array->buffer[i] = array->buffer[i + 1];
						++i;
					}
				} else if (rb == CDS_REPLACE_WITH_LAST) {
					array->buffer[i] = array->buffer[array->count - 1];
				} else {
					return CDS_INVALID_ARGUMENT;
				}
				array->count--;
				return CDS_OK;
			}
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* removes the pointer at the given index in the array */
cds_result cds_dynamic_array_remove_at(cds_dynamic_array *array, unsigned int index) {
	return cds_dynamic_array_remove_at_rb(array, index, CDS_DEFAULT_REMOVAL_BEHAVOIR);
}

/*
 * removes the pointer at the given index in the array
 * if the index is valid, the given removal behavoir is used
 */
cds_result cds_dynamic_array_remove_at_rb(cds_dynamic_array *array, unsigned int index, cds_removal_behavoir rb) {
	if (!array)
		return CDS_NULL_ARGUMENT;
	if (index < array->count) {
		if (rb == CDS_SHIFT_DOWN) {
			unsigned int idx = index;
			while (idx < array->count - 1) {
				array->buffer[idx] = array->buffer[idx + 1];
				++idx;
			}
		} else if (rb == CDS_REPLACE_WITH_LAST) {
			array->buffer[index] = array->buffer[array->count - 1];
		} else {
			return CDS_INVALID_ARGUMENT;
		}
		array->count--;
		return CDS_OK;
	} else {
		return CDS_INDEX_OUT_OF_RANGE;
	}
}

/* finds the index of the passed in data pointer and sets its value to index
 * returns CDS_OK if it is found, CDS_NOT_FOUND if it isn't, and CDS_NULL_ARGUMENT if array or index are null pointers
 */
cds_result cds_dynamic_array_find(const cds_dynamic_array *array, const void *data, unsigned int *index) {
	if (array && index) {
		unsigned int i;
		for (i = 0; i < array->count; ++i) {
			if (array->buffer[i] == data) {
				*index = i;
				return CDS_OK;
			}
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dynamic_array_find_cmp(const cds_dynamic_array *array, const void *data, unsigned int *index, cds_cmp_func cmp_func) {
	if (array && data && index && cmp_func) {
		unsigned int i;
		for (i = 0; i < array->count; ++i) {
			if ((*cmp_func)(array->buffer[i], data) == 0) {
				*index = i;
				return CDS_OK;
			}
		}
		return CDS_NOT_FOUND;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dynamic_array_reverse(cds_dynamic_array *array) {
	if (array) {
		void *tmp;
		unsigned int left = 0;
		unsigned int right = (array->count) ? array->count - 1 : 0;
		while (left < right) {
			tmp = array->buffer[left];
			array->buffer[left] = array->buffer[right];
			array->buffer[right] = tmp;
			++left;
			--right;
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}

/* */
cds_result cds_dynamic_array_iterate(const cds_dynamic_array *array, cds_visit_func visit_func) {
	if (array && visit_func) {
		unsigned int i;
		for (i = 0; i < array->count; ++i) {
			(*visit_func)(array->buffer[i]);
		}
		return CDS_OK;
	} else {
		return CDS_NULL_ARGUMENT;
	}
}
