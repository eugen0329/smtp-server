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

void delay(int ms) {
    #ifdef __linux__
        usleep(150);
    #elif WIN32
        Sleep(150);
    #endif
}

void print_file_info(char* filename, int fsize, int bufsize) {
    char file_size_msg[BUF_SIZE], file_name_msg[BUF_SIZE], buf_size_msg[BUF_SIZE];

    sprintf(file_size_msg, "%s %s", "File name: ", filename);
    sprintf(file_name_msg, "%s %d", "File size: ", fsize);
    sprintf(buf_size_msg,  "%s %d", "Buf size: ",  bufsize);

    printf("%s\n%s\n%s\n", file_size_msg, file_name_msg, buf_size_msg);
}
