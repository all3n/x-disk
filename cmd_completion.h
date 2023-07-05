#ifndef _CMD_COMPLETION_H
#define _CMD_COMPLETION_H

char *command_generator(const char *text, int state);

char **command_completion(const char *text, int start, int end);

static void init_readline();

#endif
