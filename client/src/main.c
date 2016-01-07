#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "util.h"

#define DEFAULT_PORT     6000
#define DEFAULT_ADDRESS "127.0.0.1"

char message[] = "Hello there!\n";
char buf[sizeof(message)];

int get_port(int argc, char *argv[]);
void set_address(int argc, char *argv[], char *destination, size_t len);
void receive_and_print_msg(int sock);
void receive_file(int sock);
int receive_ack(int sock);

int main (int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    int port = get_port(argc, argv);
    char server_address_string[16];
    char buf[BUF_SIZE];
    char repeat = 1;
    set_address(argc, argv, server_address_string, LENGTH_OF(server_address_string));

    printf("%s:%d\n", server_address_string, port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    /* addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); */
    addr.sin_addr.s_addr = inet_addr(server_address_string);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(2);
    }

    while(repeat) {
        printf("-> ");
        read_stdin(buf, LENGTH_OF(buf));
        if(strlen(buf) == 0) continue;
        if(!strncmp(buf, "close", LENGTH_OF(buf))) repeat = 0;
        send(sock, buf, strlen(buf), 0);

        // download file if its exists
        if(strstr(buf, "download") && receive_ack(sock)) {
            receive_file(sock);
        }

        //prints info or error mesages
        receive_and_print_msg(sock);
    }

    close(sock);

    return 0;
}

void receive_file(int sock) {
    printf("file\n");
}

int receive_ack(int sock) {
    char buf[1];

    recv(sock, buf, LENGTH_OF(buf), 0);

    return atoi(buf);
}

void receive_and_print_msg(int sock) {
    unsigned int bytes_read;
    char buf[BUF_SIZE];

    bytes_read = recv(sock, buf, LENGTH_OF(buf), 0);
    buf[bytes_read] = '\0';
    printf("<- %s\n", buf);
}

void set_address(int argc, char *argv[], char *destination, size_t len)
{
    if(argc >= 2) {
        strncpy(destination, argv[1], len);
    } else {
        strncpy(destination, DEFAULT_ADDRESS, len);
    }
}

int get_port(int argc, char *argv[])
{
    if(argc >= 3) {
        return strtol(argv[2], NULL, 10);
    } else {
        return DEFAULT_PORT;
    }
}

