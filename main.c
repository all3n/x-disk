#include "baidu_yun.h"
#include "cmd_completion.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CLFAGS:
// SOURCES: curl_utils.c utils.c file_io.c common.c baidu_yun.c
// LIBS: -lcurl -ljson-c -lreadline
// BUILD_DIR: build

#define PROMPT ">> "

void print_help() {
  printf("help:\n");
  printf("q/quit/exit: exit\n");
}
void do_user(){
  http_response *res = bdy_user_info();
  if(res == NULL){
    return;
  }
  const char * json_str = json_object_to_json_string(res->json);
  printf("%s\n", json_str);
  clean_response(res);
}

void do_list() {
  http_response *res = bdy_file_list();
  struct json_object *list_obj = json_object_object_get(res->json, "list");
  size_t len = json_object_array_length(list_obj);
  printf("len: %zu\n", len);
  for (size_t i = 0; i < len; i++) {
    struct json_object *item = json_object_array_get_idx(list_obj, i);
    struct json_object *name_obj =
        json_object_object_get(item, "server_filename");
    struct json_object *path_obj = json_object_object_get(item, "path");
    struct json_object *is_dir_obj = json_object_object_get(item, "isdir");
    struct json_object *server_mtime_obj =
        json_object_object_get(item, "server_mtime");
    struct json_object *size_obj = json_object_object_get(item, "size");
    if (name_obj != NULL && path_obj != NULL) {
      const char *name = json_object_get_string(name_obj);
      const char *path = json_object_get_string(path_obj);
      int32_t is_dir = json_object_get_int(is_dir_obj);
      int32_t mtime = json_object_get_int(server_mtime_obj);
      int64_t size = json_object_get_int64(size_obj);
      printf("%s\t%s %s %d %lld\n", path, name, is_dir ? "dir" : "file", mtime,
             size);
    }
  }
  clean_response(res);
}

int main(int argc, char *argv[]) {
  init_readline();
  global_ctx *ctx = get_global_ctx();
  init_global_ctx(ctx);
  if (!ctx->config->baidu_token) {
    printf("baidu_token is null\n");
    return -1;
  }
  // bdy_user_info();
  // bdy_quota();
  // bdy_file_list();
  // bdy_category_info();
  char *input;

  print_help();
  while ((input = readline(">> ")) != NULL) {
    int valid_input = 1;
    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0 ||
        strcmp(input, "q") == 0) {
      break;
    } else if (strcmp(input, "h") == 0) {
      print_help();
    } else if (strcmp(input, "user") == 0) {
      do_user();
    } else if (strcmp(input, "l") == 0 || strcmp(input, "list") == 0) {
      do_list();
    } else {
      valid_input = 0;
      printf("unknown command: %s\n", input);
    }
    if (valid_input) {
      add_history(input);
    }
    free(input);
  }

  // char *url = build_url("http://127.0.0.1:5001/api/post", "wd", "test",
  // NULL); printf("%s\n", url); http_request req = {.url = url, .json = true,
  // .method = POST, .data = "", .headers = NULL}; http_response res = {.data =
  // NULL, .size = 0}; int code = curl_request(&req, &res); if
  // (is_http_ok(code)) {
  //   printf("%s\n", json_object_to_json_string(res.json));
  //   printf("%s\n", json_object_get_string(json_object_object_get(res.json,
  //   "success")));
  // }

  // clean_request(&req);
  // clean_response(&res);
  clean_global_ctx(ctx);
  return EXIT_SUCCESS;
}
