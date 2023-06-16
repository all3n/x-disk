#include "utils.h"

char *get_user_path(const char *sub_path) {
  char *home_dir = getenv("HOME"); // Linux 和 Mac OS X
  if (home_dir == NULL) {
    home_dir = getenv("USERPROFILE"); // Windows
  }
  if (home_dir == NULL) {
    return NULL;
  }
  char *path = (char *)malloc(strlen(home_dir) + strlen(sub_path) + 1);
  strcpy(path, home_dir);
  strcat(path, sub_path);
  return path;
}
