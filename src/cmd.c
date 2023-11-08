#include "cmd.h"
#include "baidu_yun.h"
#include "baidu_def.h"
//#include "cmd_completion.h"
#include <readline/readline.h>
#include "common.h"
#include "utils.h"
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  // errno
  int32_t eno = json_object_get_int(json_object_object_get(res->json, "errno"));
  if (eno != 0) {
    if (eno == ERR_LIST_NOT_AUTH) {
      XLOG(INFO, "not auth");
    } else if (eno == ERR_LIST_NOT_EXIST) {
      XLOG(INFO, "not exist");
    } else {
      XLOG(INFO, "%s", bdy_get_error_msg(eno));
    }
    clean_response(res);
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
      const char *file_name =
          json_object_get_string(json_object_object_get(fmeta, "filename"));
      int64_t size =
          json_object_get_int64(json_object_object_get(fmeta, "size"));

      XLOG(INFO, "file_name %s", file_name);
      char local_path[PATH_MAX] = {0};
      char *u_local_path = expanduser(down_local_path);
      if (u_local_path == NULL) {
        strcat(local_path, file_name);
      } else if (is_dir(u_local_path)) {
        strcat(local_path, u_local_path);
        if (local_path[strlen(local_path) - 1] != '/') {
          strcat(local_path, "/");
        }
        strcat(local_path, file_name);
      } else {
        strcat(local_path, u_local_path);
      }
      free(u_local_path);
      XLOG(INFO, "download %s", local_path);
      http_response *res = bdy_download(dlink, size, local_path);
      if (is_http_ok(res->code) && res->http_code == 200) {
        XLOG(INFO, "download %s [ok]\n", local_path);
      } else {
        XLOG(ERROR, "download %s fail:%d http_code:%d\n", local_path, res->code,
             res->http_code);
      }
    }
  }
  clean_response(res);
}

void down_by_search(char *search, const char *target) {
  http_response *res = bdy_search(search);
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
    if (target) {
      down_by_fid(fs_id, target);
    } else {
      down_by_fid(fs_id, basename(search));
    }
  } else {
    XLOG(INFO, "empty search\n");
  }
  clean_response(res);
}

void do_cd(char *target) {
  global_ctx *ctx = get_global_ctx();
  if (target[0] == '/') {
    memset(ctx->pwd, 0, PATH_MAX);
    strcat(ctx->pwd, target);
  } else {
    if (ctx->pwd[strlen(ctx->pwd) - 1] != '/') {
      strcat(ctx->pwd, "/");
    }
    strcat(ctx->pwd, target);
  }
}
void do_log_level(char *level) { set_log_level(level); }
void do_get(char *token, const char *target) {
  global_ctx *ctx = get_global_ctx();
  const struct xfile *f =
      hashmap_get(ctx->files, &(struct xfile){.path = token});
  if (f) {
    down_by_fid(f->fid, target);
  } else {
    down_by_search(token, target);
  }
}
char *get_prompt(char *prompt) {
  global_ctx *ctx = get_global_ctx();
  memset(prompt, 0, PATH_MAX);
  sprintf(prompt, "[XDisk:\033[1;31m%s\033[0m]", ctx->pwd);
  return prompt;
}
void run_interactive() {
  global_ctx *ctx = get_global_ctx();
  print_help();
  char *input;
  char delimiters[] = " ";
  char *token;
  char prompt[PATH_MAX] = {0};
  while ((input = readline(get_prompt(prompt))) != NULL) {
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
    } else if (strcmp(token, "cd") == 0) {
      token = strtok(NULL, delimiters);
      do_cd(token);
    } else if (strcmp(token, "log_level") == 0) {
      token = strtok(NULL, delimiters);
      do_log_level(token);
    } else if (strcmp(token, "user") == 0) {
      do_user();
    } else if (strcmp(token, "l") == 0 || strcmp(token, "list") == 0 ||
               strcmp(token, "ls") == 0) {
      do_list();
    } else if (strcmp(token, "g") == 0 || strcmp(token, "get") == 0) {
      token = strtok(NULL, delimiters);
      const char *target = strtok(NULL, delimiters);
      do_get(token, target);
    } else {
      valid_input = 0;
      XLOG(WARNING, "unknown command: %s\n", input);
      print_help();
    }
    if (input) {
      add_history(input);
      free(input);
    }
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
    if (argc == 4) {
      down_by_search(argv[2], argv[3]);
    } else {
      down_by_search(argv[2], NULL);
    }
  }
}

