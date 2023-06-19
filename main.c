#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "baidu_yun.h"

// CLFAGS:
// SOURCES: curl_utils.c utils.c file_io.c common.c baidu_yun.c
// LIBS: -lcurl -ljson-c
// BUILD_DIR: build

int main(int argc, char *argv[]) {
  global_ctx * ctx = get_global_ctx();
  init_global_ctx(ctx);
  if(!ctx->config->baidu_token){
    printf("baidu_token is null\n");
    return -1;
  }
  // get_user_info();
  // get_quota();
  // get_file_list();
  bdy_category_info();


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
