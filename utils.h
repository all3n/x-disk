#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>
#include <string.h>

/* get user path, you should free path if your not use avoid memory leak */
char *get_user_path(const char *sub_path);
#endif // !#ifndef _UTILS_H
