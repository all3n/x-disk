#include "baidu_yun.h"
#include "cmd_completion.h"
#include "common.h"
#include "utils.h"
#include <libgen.h>
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
void do_user() {
  http_response *res = bdy_user_info();
  if (res == NULL) {
    return;
  }
  const char *json_str = json_object_to_json_string(res->json);
  XLOG(DEBUG, "%s", json_str);
  clean_response(res);
}

void do_list() {
  global_ctx *ctx = get_global_ctx();
  hashmap_clear(ctx->files, false);
  http_response *res = bdy_file_list();
  if (!is_http_ok(res->code)) {
    XLOG(ERROR, "error: %d", res->code);
    return;
  }
  struct json_object *list_obj = json_object_object_get(res->json, "list");
  size_t len = json_object_array_length(list_obj);
  XLOG(DEBUG, "len: %zu", len);
  for (size_t i = 0; i < len; i++) {
    struct json_object *item = json_object_array_get_idx(list_obj, i);
    struct json_object *name_obj =
        json_object_object_get(item, "server_filename");
    struct json_object *path_obj = json_object_object_get(item, "path");
    struct json_object *is_dir_obj = json_object_object_get(item, "isdir");
    struct json_object *server_mtime_obj =
        json_object_object_get(item, "server_mtime");
    struct json_object *size_obj = json_object_object_get(item, "size");
    struct json_object *fid_obj = json_object_object_get(item, "fs_id");
    if (name_obj != NULL && path_obj != NULL) {
      const char *name = json_object_get_string(name_obj);
      const char *path = json_object_get_string(path_obj);
      int32_t is_dir = json_object_get_int(is_dir_obj);
      int32_t mtime = json_object_get_int(server_mtime_obj);
      int64_t size = json_object_get_int64(size_obj);
      int64_t fid = json_object_get_int64(fid_obj);

      hashmap_set(
          ctx->files,
          &(struct xfile){.path = strdup(name), .fid = fid, .size = size});
      printf("\t%s\t%s %s %d %lld\n", path, name, is_dir ? "dir" : "file",
             mtime, size);
    }
  }
  clean_response(res);
}

void down_by_fid(int64_t fs_id, const char *down_local_path) {
  http_response *res = bdy_meta(fs_id, 1);
  if (res) {
    struct json_object *list_obj = json_object_object_get(res->json, "list");
    size_t len = json_object_array_length(list_obj);
    if (len) {
      struct json_object *fmeta = json_object_array_get_idx(list_obj, 0);
      const char *dlink =
          json_object_get_string(json_object_object_get(fmeta, "dlink"));
      int64_t size =
          json_object_get_int64(json_object_object_get(fmeta, "size"));
      http_response *res = bdy_download(dlink, size, down_local_path);
      if (is_http_ok(res->code)) {
        XLOG(INFO, "download %s [ok]\n", down_local_path);
      } else {
        XLOG(ERROR, "download fail:%d\n", res->code);
      }
    }
  }
  clean_response(res);
}
void do_get(char *token) {
  global_ctx *ctx = get_global_ctx();
  const struct xfile *f =
      hashmap_get(ctx->files, &(struct xfile){.path = token});
  if (f) {
    down_by_fid(f->fid, "/tmp/data");
  }
}

void run_interactive() {
  print_help();
  char *input;
  char delimiters[] = " ";
  char *token;
  while ((input = readline(">> ")) != NULL) {
    size_t input_len = strlen(input);
    if (input_len == 0) {
      continue;
    }

    int valid_input = 1;
    bool exit = false;
    token = strtok(input, delimiters);
    if (strcmp(token, "exit") == 0 || strcmp(token, "quit") == 0 ||
        strcmp(token, "q") == 0) {
      exit = true;
    } else if (strcmp(token, "h") == 0) {
      print_help();
    } else if (strcmp(token, "user") == 0) {
      do_user();
    } else if (strcmp(token, "l") == 0 || strcmp(token, "list") == 0) {
      do_list();
    } else if (strcmp(token, "g") == 0 || strcmp(token, "get") == 0) {
      token = strtok(NULL, delimiters);
      do_get(token);
    } else {
      valid_input = 0;
      XLOG(WARNING, "unknown command: %s\n", input);
      print_help();
    }
    if (valid_input) {
      add_history(input);
    }
    free(input);
    if (exit) {
      break;
    }
  }
}
void run_cmd(int argc, char *argv[]) {
  const char *cmd = argv[1];
  if (strcmp(cmd, "d") == 0) {
    if (argc < 3) {
      XLOG(ERROR, "invalid args\n");
      return;
    }
    char *file_path = argv[2];
    http_response *res = bdy_search(file_path);
    if (!is_http_ok(res->code)) {
      XLOG(ERROR, "search fail!:%d\n", res->code);
      return;
    }
    struct json_object *list_obj = json_object_object_get(res->json, "list");
    size_t len = json_object_array_length(list_obj);
    if (len > 0) {
      // get first
      struct json_object *item = json_object_array_get_idx(list_obj, 0);
      struct json_object *fs_id_obj = json_object_object_get(item, "fs_id");
      int64_t fs_id = json_object_get_int64(fs_id_obj);
      XLOG(DEBUG, "fs_id:%lld\n", fs_id);
      if (argc == 4) {
        down_by_fid(fs_id, argv[3]);
      } else {
        down_by_fid(fs_id, basename(file_path));
      }
    } else {
      XLOG(INFO, "empty search\n");
    }
    clean_response(res);
  }
}

int main(int argc, char *argv[]) {
  init_readline();
  global_ctx *ctx = get_global_ctx();
  init_global_ctx(ctx);
  if (!ctx->config->baidu_token) {
    XLOG(WARNING, "baidu_token is null");
    return -1;
  }
  if (argc == 1) {
    run_interactive();
  } else if (strcmp(argv[1], "-i") == 0) {
    run_interactive();
  } else {
    run_cmd(argc, argv);
  }

  clean_global_ctx(ctx);
  return EXIT_SUCCESS;
}
