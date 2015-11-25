#include "tui.h"

static char stdin_buf[_STDIN_BUF_LEN];

const char* _tui_commands[] = {
    "echo",
    "time",
    "close"
};

command_t read_command()
{
    if(read_stdin(stdin_buf, _STDIN_BUF_LEN) == EXIT_FAILURE) {
        return CMD_UNKNOWN;
    }
    if(_STR_EQ(stdin_buf, _tui_commands[CMD_ECHO])) {
        return CMD_ECHO;
    } else if(_STR_EQ(stdin_buf, _tui_commands[CMD_TIME])) {
        return CMD_TIME;
    } else if(_STR_EQ(stdin_buf, _tui_commands[CMD_CLOSE])) {
        return CMD_CLOSE;
    }

    return CMD_UNKNOWN;
}
