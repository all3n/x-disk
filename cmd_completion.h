#ifndef _CMD_COMPLETION_H
#define _CMD_COMPLETION_H

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>

char *command_generator(const char *text, int state);
char **command_completion(const char *text, int start, int end);
void init_readline();

#endif
