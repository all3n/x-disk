#include "baidu_yun.h"
#include "common.h"
#include "curl_utils.h"
// https://pan.baidu.com/union/doc/Kl4gsu388

/*
 *  get_user_info account/avatar/vip_type
 *  https://pan.baidu.com/union/doc/pksg0s9ns
 */
int get_user_info() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  char *url = build_url2(BDY_API_BASE, "/rest/2.0/xpan/nas", "method", "uinfo",
                         "openapi", "xpansdk", "access_token", baidu_token);
  http_request req = {.url = url, .json = true, .method = GET};
  http_response res = {.data = NULL, .size = 0};
  int code = curl_request(&req, &res);
  if (is_http_ok(code)) {
    if(ctx->user_info){
      // clean last user_info
      json_object_put(ctx->user_info);
      ctx->user_info = NULL;
    }
    ctx->user_info = res.json;
    printf("%s\n", json_object_to_json_string(res.json));
  }
  clean_request(&req);
  // release in global
  // clean_response(&res);
  return 0;
}

/*
 *  get_quota of used/total
 *  https://pan.baidu.com/union/doc/Cksg0s9ic
 */
int get_quota() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  char *url = build_url2(BDY_API_BASE, "/api/quota", "check_free", "1",
                         "checkexpire", "1", "access_token", baidu_token);
  http_request req = {.url = url, .json = true, .method = GET};
  http_response res = {.data = NULL, .size = 0};
  int code = curl_request(&req, &res);
  if (is_http_ok(code)) {
    printf("%s\n", json_object_to_json_string(res.json));
  }
  clean_request(&req);
  // release in global
  clean_response(&res);
  return 0;

  return 0;
}