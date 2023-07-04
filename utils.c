#include "utils.h"
#include <stdio.h>

int get_log_level() {
  if (LOG_LEVEL == -1) {
    char *log_level_str = getenv("LOG_LEVEL");
    if (log_level_str != NULL) {
      if (strcasecmp(log_level_str, "DEBUG") == 0) {
        LOG_LEVEL = XLOG_DEBUG;
      } else if (strcasecmp(log_level_str, "INFO") == 0) {
        LOG_LEVEL = XLOG_INFO;
      } else if (strcasecmp(log_level_str, "WARNING") == 0) {
        LOG_LEVEL = XLOG_WARNING;
      } else if (strcasecmp(log_level_str, "ERROR") == 0) {
        LOG_LEVEL = XLOG_ERROR;
      } else {
        LOG_LEVEL = atoi(log_level_str);
      }
    } else {
      LOG_LEVEL = XLOG_INFO;
    }
    printf("LOG_LEVEL: %d\n", LOG_LEVEL);
  }
  return LOG_LEVEL;
}
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
