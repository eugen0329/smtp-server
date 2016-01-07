#ifndef UTIL_H_RMKDBCTA
#define UTIL_H_RMKDBCTA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COUNT_OF(STATIC_COLLECTION) \
    (sizeof(STATIC_COLLECTION) / sizeof((STATIC_COLLECTION)[0]))
#define SIZE_OF(STATIC_COLLECTION) COUNT_OF(STATIC_COLLECTION)
#define LENGTH_OF(STATIC_COLLECTION) COUNT_OF(STATIC_COLLECTION)
#define EMPTY_FLAGS 0

unsigned char read_stdin(char* buf, size_t buflen);

#endif /* end of include guard: UTIL_H_RMKDBCTA */

