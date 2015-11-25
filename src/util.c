#include "util.h"

unsigned char read_stdin(char* buf, size_t buflen)
{
    if(fgets(buf, buflen, stdin) == NULL) {
        return EXIT_FAILURE;
    } else {
        buf[strlen(buf) - 1] = '\0';
    }
    return EXIT_SUCCESS;
}
