#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "util.h"
#include "tui.h"

#define SOC_ERR -1
#define SERV_PORT 5555
#define SERV_QUEUE_LEN 1
#define BUF_SIZE 1024
#define WONG_SOC(soc_fd) (soc_fd < 0)
#define EMPTY_FLAGS 0

typedef int socket_t;
typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;

int main(int argc, char *argv[])
{
    socket_t server_fd, client_fd;
    sockaddr_in_t server_address;
    unsigned int bytes_read;

    char buf[BUF_SIZE];


    // AF_INET: IPv4 internet protocols,
    // SOCK_STREAM: sequenced two-way byte streams
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == SOC_ERR) {
        perror("socket creation failure");
        return EXIT_FAILURE;
    }


    memset(&server_address, 0, sizeof(sockaddr_in_t));
    memset(&buf, 0, sizeof(unsigned char));
    server_address.sin_family = AF_INET; // address family: alsways set to AF_INET
    // s_addr: address in network,
    // INADDR_ANY: bound soc to all local interfaces,
    // htonl: converts uint host to network bytes order
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // sin_port: port(network byte order)
    // htons: converts ushort host to network bytes order
    server_address.sin_port = htons(SERV_PORT);
    bind(server_fd, (sockaddr_t *) &server_address, sizeof(sockaddr_in_t));

    // Create requests queue, suspend socket
    listen(server_fd, SERV_QUEUE_LEN);

    while (true) {
        // $2: client_addr, $3: client_addr_len
        client_fd = accept(server_fd, (sockaddr_t *) 0, NULL);
        if(WRONG_SOC(client_fd)) {
            perrror("accept");
            exit(EXIT_FAILURE);
        }

        while(1) {
            bytes_read = recv(client_fd, buf, BUF_SIZE, EMPTY_FLAGS);
            if(bytes_read <= 0) break;
            send(client_sock, buf, bytes_read, EMPTY_FLAGS);
        }

        close(client_fd);
    }

    /* while(true) { */
    /*     switch(read_command()) { */
    /*         case CMD_UNKNOWN: */
    /*             break; */
    /*         case CMD_ECHO: */
    /*             printf("echo\n"); */
    /*             break; */
    /*         case CMD_TIME: */
    /*             printf("time\n"); */
    /*             break; */
    /*         case CMD_CLOSE: */
    /*             printf("close\n"); */
    /*             break; */
    /*         default: */
    /*             break; */
    /*     } */
    /* } */

    return EXIT_SUCCESS;
}
