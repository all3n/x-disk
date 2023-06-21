#include <curl/curl.h>
#include <stdio.h>

#include "common.h"
#include "file_io.h"
#include "utils.h"
void destroy(void *p) {
  printf("destroy\n");
  if (p) {
    free(p);
  }
}

void parse_config(global_ctx *ctx) {
  app_config *cfg = (app_config *)malloc(sizeof(app_config));
  cfg->baidu_token =
      json_object_get_string(json_object_object_get(ctx->json, "baidu_token"));

  ctx->config = cfg;
}
int init_global_ctx(global_ctx *ctx) {
  ctx->path = get_user_path(CONFIG_PATH);
  if (ctx->path == NULL) {
    printf("get_user_path failed\n");
    return -1;
  }
  printf("config_path:%s\n", ctx->path);
  ctx->json = load_json_from_file(ctx->path);
  parse_config(ctx);
  // curl global init
  curl_global_init(CURL_GLOBAL_DEFAULT);
  return 0;
}
void clean_config(global_ctx *ctx) {
  if (ctx->config == NULL) {
    return;
  }
  free(ctx->config);
  ctx->config = NULL;
}
void clean_global_ctx(global_ctx *ctx) {
  if (ctx->path) {
    free(ctx->path);
    ctx->path = NULL;
  }
  if (ctx->json) {
    json_object_put(ctx->json);
    ctx->json = NULL;
  }

  if (ctx->config) {
    clean_config(ctx);
    free(ctx->config);
    ctx->config = NULL;
  }
  if (ctx->user_info) {
    free(ctx->user_info);
    ctx->user_info = NULL;
  }
  curl_global_cleanup();
}

global_ctx *get_global_ctx() {
  static global_ctx g_ctx;
  return &g_ctx;
};
