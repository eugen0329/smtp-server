#ifndef TRANSFER_H_8EOXMTNZ
#define TRANSFER_H_8EOXMTNZ

#define ACK "1"
#define ERROR "0"
#define NO_ENTRY_ERR "No such file"
#define NO_MEM_ERR "Not enough memory"

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>

#include "util.h"

void send_file(int client_fd, char *filename);
void no_file_error(int client_fd);
void no_mem_error(int client_fd);
void send_flag(int client_fd, char* flag);
void print_and_send_file_info(int client_fd, char* filename, int fsize);

#endif /* end of include guard: TRANSFER_H_8EOXMTNZ */
