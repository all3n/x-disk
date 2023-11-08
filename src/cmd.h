#ifndef _CMD_H
#define _CMD_H
#include <stdlib.h>
void print_help();
void do_user();
void do_list();
void down_by_fid(int64_t fs_id, const char *down_local_path);
void down_by_search(char *search, const char *target);
void do_cd(char *target);
char *get_prompt(char *prompt);
void run_interactive();
void run_cmd(int argc, char *argv[]);

#endif
