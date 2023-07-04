#ifndef _COMMON_H
#define _COMMON_H
#include "hashmap.h"
#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_PATH "/.config/x-disk/config.json"
void destroy(void *p);
#define AUTO_FREE __attribute__((cleanup(destroy)))

struct xfile {
  char *path;
  int64_t fid;
  int64_t size;
};
typedef struct app_config {
  const char *baidu_token;

} app_config;
typedef struct global_ctx {
  char *path;
  app_config *config;
  struct json_object *json;
  struct json_object *user_info;
  struct hashmap *files;
} global_ctx;

int init_global_ctx(global_ctx *ctx);
void clean_global_ctx(global_ctx *ctx);
global_ctx *get_global_ctx();

int xfile_compare(const void *a, const void *b, void *udata);
bool xfile_iter(const void *item, void *udata);
uint64_t xfile_hash(const void *item, uint64_t seed0, uint64_t seed1);

#endif
