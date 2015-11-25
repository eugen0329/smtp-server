#ifndef TUI_H_CWFM4G5T
#define TUI_H_CWFM4G5T

#include <string.h>
#include <stdlib.h>
#include "util.h"

#define _STR_EQ(s1, s2) (!strncmp(s1, s2, strlen(s2)))

#define _STDIN_BUF_LEN 128

typedef enum { CMD_UNKNOWN = -1, CMD_ECHO, CMD_TIME, CMD_CLOSE } command_t;

extern const char* _tui_commands[];

command_t read_command();

#endif /* end of include guard: TUI_H_CWFM4G5T */
