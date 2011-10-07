#include "cds_core.h"
/*
int cds_log(const char *format, ...) {
	va_list argp;
	va_start(argp, format);
	int n = vprintf(format, argp);
	va_end(argp)
	return n;
}
*/

// the default assertion reporting handler
void cds_default_assertion_failure_reporter(const char *condition, const char *file, int line, const char *message, va_list  argp) {
	printf("Assertion Failed: %s\n", condition);
	printf("In file: %s at line: %d\n", file, line);
	if (message) {
		//va_start(argp, message);
		vprintf(message, argp);
		//va_end(argp);
	}
}

// set to the default
cds_assertion_failure_reporter_func cds_assertion_reporter = &cds_default_assertion_failure_reporter;

// called by the macro on an assert
// reports the failure thru the function pointer and exits the program
void cds_on_assertion_failure(const char *condition, const char *file, int line, const char *message, ...) {
	// how do we pass an arg list??
	if (cds_assertion_reporter) {
		va_list argp;
		va_start( argp, message);
		(*cds_assertion_reporter)(condition, file, line, message, argp);
		va_end(argp);
	}
	exit(EXIT_FAILURE);
}

// determines if the result is an error
int cds_is_error(cds_result r) {
	return r != CDS_OK && r != CDS_NOT_FOUND;
}

// puts a human readable string of the error into str
// returns the length of the string or -1 if the cds_result was not found
int cds_result_string(cds_result r, char *str) {
	int n = -1;
	switch (r) {
		case CDS_OK:
			n = sprintf(str, "CDS_OK");
			break;
		case CDS_NOT_FOUND:
			n = sprintf(str, "CDS_NOT_FOUND");
			break;
		case CDS_INVALID_ARGUMENT:
			n = sprintf(str, "CDS_INVALID_ARGUMENT");
			break;
		case CDS_INVALID_OPERATION:
			n = sprintf(str, "CDS_INVALID_OPERATION");
			break;
		case CDS_NULL_ARGUMENT:
			n = sprintf(str, "CDS_NULL_ARGUMENT");
		case CDS_INDEX_OUT_OF_RANGE:
			n = sprintf(str, "CDS_INDEX_OUT_OF_RANGE");
			break;
		case CDS_UNDERFLOW:
			n = sprintf(str, "CDS_UNDERFLOW");
			break;
		case CDS_OVERFLOW:
			n = sprintf(str, "CDS_OVERFLOW");
			break;
		case CDS_BAD_ALLOC:
			n = sprintf(str, "CDS_BAD_ALLOC");
			break;
		case CDS_UNIMPLEMENTED:
			n = sprintf(str, "CDS_UNIMPLEMENTED");
			break;
		default:
			n = sprintf(str, "cds_result with value: %d was not found!", r);
	}
	return n;
}

//
int cds_error_check(cds_result r) {
	int isError = cds_is_error(r);
	if (isError) {
		char errorStr[CDS_MAX_ERR_STR_LEN];
		cds_result_string(r, errorStr);
		printf("CDS Error: %s\n", errorStr);
	}
	return isError;
}

// use the standard library by default
cds_alloc_func cds_allocator = NULL;
cds_realloc_func cds_reallocator = NULL;
cds_free_func cds_freer = NULL;


// sets the address of the alloc func to use in cds_alloc
void cds_set_alloc(cds_alloc_func alloc_func) {
	cds_allocator = alloc_func;
}

// sets the address of the realloc func to use in cds_realloc
void cds_set_realloc(cds_realloc_func realloc_func) {
	cds_reallocator = realloc_func;
}

// sets the address of the free func to use in cds_free
void cds_set_free(cds_free_func free_func) {
	cds_freer = free_func;
}

// gets the address of the current alloc function
cds_alloc_func cds_get_alloc() {
	return (cds_allocator) ? cds_allocator : &malloc;
}

// gets the address of the current realloc function
cds_realloc_func cds_get_realloc() {
	return (cds_reallocator) ? cds_reallocator : &realloc;
}

// gets the address of the current free function
cds_free_func cds_get_free() {
	return (cds_freer) ? cds_free : &free;
}

// allocates a block of memory with the given size
void * cds_alloc(size_t size) {
	if (cds_allocator)
		return (*cds_allocator)(size);
	else
		return malloc(size);
}

// reallocates a block of memory to the given size
void * cds_realloc(void *ptr, size_t size) {
	if (cds_reallocator)
		return (*cds_reallocator)(ptr, size);
	else
		return realloc(ptr, size);
}

// frees a given block of memory
void cds_free(void *ptr) {
	if (cds_freer)
		return (*cds_freer)(ptr);
	else
		return free(ptr);
}