#include "cds_core.h"

#ifndef CDS_DEFAULT_LOGGER
/** The default logging func name used if none are specified */
#define CDS_DEFAULT_LOGGER vprintf
#endif

static cds_log_func cds_logger = &CDS_DEFAULT_LOGGER;

cds_log_func cds_get_log() {
	return cds_logger;
}

void cds_set_log(cds_log_func lf) {
	cds_logger = lf;
}

int cds_log(const char *format, ...) {
	va_list argp;
	va_start(argp, format);
	int n; 
	if (cds_logger)
		n = (*cds_logger)(format, argp);
	else
		n = CDS_DEFAULT_LOGGER(format, argp);
	va_end(argp);
	return n;
}

void cds_default_assertion_failure_reporter(const char *condition, const char *file, int line, const char *message, va_list argp) {
	printf("Assertion Failed: %s\n", condition);
	printf("In file: %s at line: %d\n", file, line);
	if (message) {
		if (cds_logger)
			(*cds_logger)(message, argp);
		else
			CDS_DEFAULT_LOGGER(message, argp);
	}
}

#ifndef CDS_DEFAULT_ASSERTION_REPORTER
/** The default assertion reporter func name used if none are specified */
#define CDS_DEFAULT_ASSERTION_REPORTER cds_default_assertion_failure_reporter
#endif

cds_assertion_failure_reporter_func cds_assertion_reporter = &CDS_DEFAULT_ASSERTION_REPORTER;

cds_assertion_failure_reporter_func cds_get_assertion_reporter() {
	return cds_assertion_reporter;
}

void cds_set_assertion_reporter(cds_assertion_failure_reporter_func reporter_func) {
	cds_assertion_reporter = reporter_func;
}

void cds_on_assertion_failure(const char *condition, const char *file, int line, const char *message, ...) {
	/* how do we pass an arg list?? */
	if (cds_assertion_reporter) {
		va_list argp;
		va_start( argp, message);
		(*cds_assertion_reporter)(condition, file, line, message, argp);
		va_end(argp);
	}
	exit(EXIT_FAILURE);
}

int cds_is_error(cds_result r) {
	return r != CDS_OK && r != CDS_NOT_FOUND && r != CDS_DUPLICATE_VALUE;
}

int cds_result_string(cds_result cr, char *str) {
	int n = -1;
	switch (cr) {
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
			break;
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
		case CDS_DUPLICATE_VALUE:
			n = sprintf(str, "CDS_DUPLICATE_VALUE");
			break;
		default:
			n = sprintf(str, "cds_result with value: %d was not found!", cr);
			break;
	}
	return n;
}

int cds_error_check(cds_result cr) {
	int isError = cds_is_error(cr);
	if (isError) {
		char errorStr[CDS_MAX_ERR_STR_LEN];
		cds_result_string(cr, errorStr);
		printf("CDS Error: %s\n", errorStr);
	}
	return isError;
}

int cds_not_ok_check(cds_result cr) {
	int isNotOkay = cr != CDS_OK;
	if (!isNotOkay) {
		char errorStr[CDS_MAX_ERR_STR_LEN];
		cds_result_string(cr, errorStr);
		printf("CDS Error: %s\n", errorStr);
	}
	return isNotOkay;
}

#ifndef CDS_DEFAULT_ALLOCATOR
/** The default allocator func name used if none are specified */
#define CDS_DEFAULT_ALLOCATOR malloc
#endif

#ifndef CDS_DEFAULT_REALLOCATOR
/** The default reallocator func name used if none are specified */
#define CDS_DEFAULT_REALLOCATOR realloc
#endif

#ifndef CDS_DEFAULT_FREER
/** The default free func name used if none are specified */
#define CDS_DEFAULT_FREER free
#endif

cds_alloc_func cds_allocator = &CDS_DEFAULT_ALLOCATOR;
cds_realloc_func cds_reallocator = &CDS_DEFAULT_REALLOCATOR;
cds_free_func cds_freer = &CDS_DEFAULT_FREER;


void cds_set_alloc(cds_alloc_func alloc_func) {
	cds_allocator = alloc_func;
}

void cds_set_realloc(cds_realloc_func realloc_func) {
	cds_reallocator = realloc_func;
}

void cds_set_free(cds_free_func free_func) {
	cds_freer = free_func;
}

cds_alloc_func cds_get_alloc() {
	return (cds_allocator) ? cds_allocator : &CDS_DEFAULT_ALLOCATOR;
}

cds_realloc_func cds_get_realloc() {
	return (cds_reallocator) ? cds_reallocator : &CDS_DEFAULT_REALLOCATOR;
}

cds_free_func cds_get_free() {
	return (cds_freer) ? cds_freer : &CDS_DEFAULT_FREER;
}

void * cds_alloc(size_t size) {
	if (cds_allocator)
		return (*cds_allocator)(size);
	else
		return CDS_DEFAULT_ALLOCATOR(size);
}

void * cds_realloc(void *ptr, size_t size) {
	if (cds_reallocator)
		return (*cds_reallocator)(ptr, size);
	else
		return CDS_DEFAULT_REALLOCATOR(ptr, size);
}

void cds_free(void *ptr) {
	if (cds_freer)
		return (*cds_freer)(ptr);
	else
		return CDS_DEFAULT_FREER(ptr);
}
