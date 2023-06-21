#ifndef _COMMON_H
#define _COMMON_H
#include <json-c/json.h>

#define CONFIG_PATH "/.config/x-disk/config.json"
void destroy(void *p);
#define AUTO_FREE __attribute__((cleanup(destroy)))

typedef struct app_config {
  const char *baidu_token;

} app_config;
typedef struct global_ctx {
  char *path;
  app_config *config;
  struct json_object *json;
  struct json_object *user_info;
} global_ctx;

int init_global_ctx(global_ctx *ctx);
void clean_global_ctx(global_ctx *ctx);

global_ctx *get_global_ctx();

#endif
