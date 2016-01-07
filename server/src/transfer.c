#include "transfer.h"


int send_file(int soc, char *filename)
{
    void *buf;
    socklen_t optlen;
    size_t bytes_read, fsize, bufsize = BUF_SIZE;
    FILE *f;

    if((f = fopen(filename, "rb")) == NULL) return ENOENT;

    // SOL_SOCKET: set options at the SOcket Level

    // Set buffer size
    setsockopt(soc, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));

    // Get buffer size and write it to bufsize
    optlen = sizeof(bufsize);
    getsockopt(soc, SOL_SOCKET, SO_SNDBUF, &bufsize, &optlen);

    // kernel needs harf of the buf size
#ifdef __linux__
    bufsize /= 2;
#endif

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
