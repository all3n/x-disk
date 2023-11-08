#ifndef _COMMON_H
#define _COMMON_H
#include "hashmap.h"
#include <json-c/json.h>
#include <stdio.h>
#include <string.h>
#include <ch_curl_utils.h>
#ifdef __MINGW32__
#include <limits.h>
#else
#include <sys/syslimits.h>
#endif

typedef struct json_object * X_JSON;

#define PROMPT "XDISK>> "
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
  const char *app_key;
} app_config;
typedef struct global_ctx {
  char *config_path;
  char pwd[PATH_MAX];
  app_config *config;
  char *device_code;
  X_JSON json;
  X_JSON user_info;
  struct hashmap *files;
  struct ch_requests *requests;
} global_ctx;

int init_global_ctx(global_ctx *ctx);
void clean_global_ctx(global_ctx *ctx);
global_ctx *get_global_ctx();

int xfile_compare(const void *a, const void *b, void *udata);
bool xfile_iter(const void *item, void *udata);
uint64_t xfile_hash(const void *item, uint64_t seed0, uint64_t seed1);

#endif
