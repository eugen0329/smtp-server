#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "util.h"

#define DEFAULT_PORT 6000
#define DEFAULT_ADDRESS "127.0.0.1"

#define SOC_ERR -1
#define SERV_QUEUE_LEN 1
#define WRONG_SOC(soc_fd) (soc_fd < 0)
#define UNKNOWN_COMMAND_ERR "Unknown command"
#define EMPTY_ECHO_ERR "Empty echo string"

typedef int socket_t;
typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;

int get_port(int argc, char *argv[]);
void set_address(int argc, char *argv[], char *destination, size_t len);
char * server_time();


void server_time_route(int client_fd, char *buf, size_t bytes_read);
void error_route(int client_fd, char *buf, size_t bytes_read);
void echo_route(int client_fd, char *buf, size_t bytes_read);

void download_route(int client_fd, char *buf, size_t bytes_read);
void close_route(int client_fd, char *buf, size_t bytes_read);

int main(int argc, char *argv[])
{
    socket_t server_fd, client_fd;
    sockaddr_in_t server_address;
    unsigned int bytes_read;
    char buf[BUF_SIZE];

    char server_address_string[16];
    int port = get_port(argc, argv);
    set_address(argc, argv, server_address_string, LENGTH_OF(server_address_string));
    printf("Listening on %s:%d\n",
            server_address_string, port);

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
    /* server_address.sin_addr.s_addr = htonl(inaddr_any); */
    server_address.sin_addr.s_addr = inet_addr(server_address_string);

    // sin_port: port(network byte order)
    // htons: converts ushort host to network bytes order
    server_address.sin_port = htons(port);
    bind(server_fd, (sockaddr_t *) &server_address, sizeof(sockaddr_in_t));

    // Create requests queue, suspend socket
    listen(server_fd, SERV_QUEUE_LEN);

    while (true) {
        // $2: client_addr, $3: client_addr_len
        client_fd = accept(server_fd, (sockaddr_t *) 0, NULL);
        if(WRONG_SOC(client_fd)) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Accepted clinet:\n");

        while(true) {
            bytes_read = recv(client_fd, buf, BUF_SIZE - 1, EMPTY_FLAGS);
            if(bytes_read <= 0) break;
            buf[bytes_read] = '\0';

            printf("<- %s\n", buf);
            if(!strncmp(buf, "time", LENGTH_OF(buf))) {
                server_time_route(client_fd, buf, bytes_read);
            } else if(strstr(buf, "echo")) {
                echo_route(client_fd, buf, bytes_read);
            } else if(strstr(buf, "download")) {
                download_route(client_fd, buf, bytes_read);
            } else if(!strncmp(buf, "close", LENGTH_OF(buf))) {
                close_route(client_fd, buf, bytes_read);
                break;
            } else {
                error_route(client_fd, buf, bytes_read);
            }
        }

        close(client_fd);
        printf("Close client fd\n");
    }
    close(server_fd);

    return EXIT_SUCCESS;
}

void download_route(int client_fd, char *buf, size_t bytes_read)
{
    // TODO: buf parsing
    send_file(client_fd, "a.pdf");
}

void close_route(int client_fd, char *buf, size_t bytes_read)
{
    char msg[] = "Connection is closed";
    printf("-> %s (%zu bytes)\n", msg, strlen(msg));
    send(client_fd, msg, strlen(msg), EMPTY_FLAGS);
}

void echo_route(int client_fd, char *buf, size_t bytes_read)
{
    char *p = strstr(buf, "echo");
    p += LENGTH_OF("echo") - 1;
    if(strlen(p)) {
        printf("-> %s (%zu bytes)\n", p, strlen(p));
        send(client_fd, p, strlen(p), EMPTY_FLAGS);
    } else {
        printf("-> %s (%zu bytes)\n", EMPTY_ECHO_ERR, sizeof(EMPTY_ECHO_ERR));
        send(client_fd, EMPTY_ECHO_ERR, strlen(EMPTY_ECHO_ERR), EMPTY_FLAGS);
    }
}

void error_route(int client_fd, char *buf, size_t bytes_read)
{
    printf("-> %s (%zu bytes)\n", UNKNOWN_COMMAND_ERR, sizeof(UNKNOWN_COMMAND_ERR));
    send(client_fd, UNKNOWN_COMMAND_ERR, strlen(UNKNOWN_COMMAND_ERR), EMPTY_FLAGS);
}

void server_time_route(int client_fd, char *buf, size_t bytes_read)
{
    char *time_str;
    time_str = server_time();
    time_str[strlen(time_str) - 1] = '\0';
    printf("-> %s (%zu bytes)\n", time_str, strlen(time_str));
    send(client_fd, time_str, strlen(time_str), EMPTY_FLAGS);
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

char * server_time()
{
    time_t current_time;
    current_time = time(&current_time);
    return ctime(&current_time);
}
/* printf(" --- %d bytes read\n", buffer, bytes_read); */
