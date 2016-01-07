#include "transfer.h"


int send_file(int soc, char *filename)
{
    void *buf;
    size_t bytes_read, fsize, bufsize = 512;
    FILE *f;

    if((f = fopen(filename, "rb")) == NULL) return ENOENT;
    if((buf = malloc(bufsize)) == NULL) return ENOMEM;

    fseek(f, 0, SEEK_END); // seek to end of file
    fsize = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file

    do {
        bytes_read = fread(buf, 1, bufsize, f);
        send(soc, buf, bytes_read, EMPTY_FLAGS);
    } while(bytes_read >= bufsize - 1);

    return 0;
}