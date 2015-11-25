#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STDIN_BUF_LENGTH 128
#define STR_EQ(s1, s2) (!strncmp(s1, s2, strlen(s2)))

typedef enum { CMD_ECHO, CMD_TIME, CMD_CLOSE } commands_t;
const char* commands[] = {
    "echo",
    "time",
    "close"
};

unsigned char read_stdin(char* buf, size_t buflen)
{
    if(fgets(buf, buflen, stdin) != NULL) {
        return EXIT_FAILURE;
    } else {
        buf[strlen(buf) - 1] = '\0';
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    char stdin_buf[STDIN_BUF_LENGTH];

    while(true) {
        if(read_stdin(stdin_buf, STDIN_BUF_LENGTH) == EXIT_FAILURE) {
            printf("Wrong cmd len\n");
            continue;
        }
        if(STR_EQ(stdin_buf, commands[CMD_ECHO])) {
            printf("echo\n");
        } else if(STR_EQ(stdin_buf, commands[CMD_TIME])) {
            printf("time\n");
        } else if(STR_EQ(stdin_buf, commands[CMD_CLOSE])) {
            printf("close\n");
        } else {
            printf("Wrong cmd\n");
        }
    }

    return EXIT_SUCCESS;
}
