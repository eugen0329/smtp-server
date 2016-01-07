#ifndef UTIL_H_RMKDBCTA
#define UTIL_H_RMKDBCTA

#ifdef __linux__
  #include <unistd.h>
#elif WIN32
  #include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define COUNT_OF(STATIC_COLLECTION) \
    (sizeof(STATIC_COLLECTION) / sizeof((STATIC_COLLECTION)[0]))
#define SIZE_OF(STATIC_COLLECTION) COUNT_OF(STATIC_COLLECTION)
#define LENGTH_OF(STATIC_COLLECTION) COUNT_OF(STATIC_COLLECTION)
#define EMPTY_FLAGS 0
#define BUF_SIZE 2304

unsigned char read_stdin(char* buf, size_t buflen);
void print_file_info(char* filename, int fsize, int bufsize);
void delay(int ms);

#endif /* end of include guard: UTIL_H_RMKDBCTA */

