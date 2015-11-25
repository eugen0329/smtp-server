#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>


#include "util.h"
#include "tui.h"

typedef int socket_t;

int main(int argc, char *argv[])
{
    while(true) {
        switch(read_command()) {
            case CMD_UNKNOWN:
                break;
            case CMD_ECHO:
                printf("echo\n");
                break;
            case CMD_TIME:
                printf("time\n");
                break;
            case CMD_CLOSE:
                printf("close\n");
                break;
            default:
                break;
        }
    }

    return EXIT_SUCCESS;
}
