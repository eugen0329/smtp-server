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

    print_and_send_file_info(client_fd, filename, fsize);

    do {
        bytes_read = fread(buf, 1, bufsize, f);
        send(client_fd, buf, bytes_read, EMPTY_FLAGS);
    } while(bytes_read >= bufsize - 1);
}

void print_and_send_file_info(int client_fd, char* filename, int fsize) {
    char file_size_msg[BUF_SIZE], file_name_msg[BUF_SIZE];

    sprintf(file_size_msg, "%s %s", "File name: ", filename);
    sprintf(file_name_msg, "%s %d", "File size: ", fsize);

    printf("%s\n", file_size_msg);
    printf("%s\n", file_name_msg);

    send(client_fd, file_name_msg, strlen(file_name_msg), EMPTY_FLAGS);
    delay(150);
    send(client_fd, file_size_msg, strlen(file_size_msg), EMPTY_FLAGS);
    delay(150);
}

void send_flag(int client_fd, char* flag) {
    send(client_fd, flag, strlen(flag), EMPTY_FLAGS);

    // add sleep for sending delay
    delay(150);
}

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
