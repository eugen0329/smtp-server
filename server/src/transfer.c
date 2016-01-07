#include "transfer.h"


void send_file(int client_fd, char *filename)
{
    void *buf;
    socklen_t optlen;
    size_t bytes_read, fsize, bufsize = BUF_SIZE;
    FILE *f;

    if((f = fopen(filename, "rb")) == NULL) {
        no_file_error(client_fd);
        return;
    }

    // SOL_SOCKET: set options at the SOcket Level(SO_SNDBUF - buf size)
    setsockopt(client_fd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));

    // Get buffer size and write it to bufsize
    optlen = sizeof(bufsize);
    getsockopt(client_fd, SOL_SOCKET, SO_SNDBUF, &bufsize, &optlen);

    // kernel needs harf of the buf size
#ifdef __linux__
    bufsize /= 2;
#endif

    if((buf = malloc(bufsize)) == NULL) {
        no_mem_error(client_fd);
        return;
    }

    // send ack to client => he will start receiving file
    send_flag(client_fd, ACK);

    fseek(f, 0, SEEK_END); // seek to end of file
    fsize = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file

    printf("File name: %s\n", filename);
    printf("File size: %i\n", fsize);

    do {
        bytes_read = fread(buf, 1, bufsize, f);
        send(client_fd, buf, bytes_read, EMPTY_FLAGS);
    } while(bytes_read >= bufsize - 1);
}

void send_flag(int client_fd, char* flag) {
    send(client_fd, flag, strlen(flag), EMPTY_FLAGS);

    // add sleep for sending delay
#ifdef __linux__
    usleep(150);
#elif WIN32
    Sleep(150);
#endif
}

// void send_ack(int client_fd) {
//     send(client_fd, ACK, strlen(ACK), EMPTY_FLAGS);

// }

void no_file_error(int client_fd) {
    send_flag(client_fd, ERROR);

    printf("-> %s (%zu bytes)\n", NO_ENTRY_ERR, strlen(NO_ENTRY_ERR));
    send(client_fd, NO_ENTRY_ERR, strlen(NO_ENTRY_ERR), EMPTY_FLAGS);
}

void no_mem_error(int client_fd) {
    send_flag(client_fd, ERROR);

    printf("-> %s (%zu bytes)\n", NO_MEM_ERR, strlen(NO_MEM_ERR));
    send(client_fd, NO_MEM_ERR, strlen(NO_MEM_ERR), EMPTY_FLAGS);
}
