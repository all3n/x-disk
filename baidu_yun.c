#include "baidu_yun.h"
#include "common.h"
#include "curl_utils.h"
#include "utils.h"
// https://pan.baidu.com/union/doc/Kl4gsu388

/*
 *  get_user_info account/avatar/vip_type
 *  https://pan.baidu.com/union/doc/pksg0s9ns
 */
http_response *bdy_user_info() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  char *url =
      build_url2(BDY_API_BASE, "/rest/2.0/xpan/nas", "method", "uinfo",
                 "openapi", "xpansdk", "access_token", baidu_token, NULL);
  http_request req = {.url = url, .json = true, .method = GET};
  http_response *res = malloc(sizeof(http_response));
  memset(res, 0, sizeof(http_response));
  int code = curl_request(&req, res);
  if (is_http_ok(code)) {
    if (ctx->user_info) {
      // clean last user_info
      json_object_put(ctx->user_info);
      ctx->user_info = NULL;
    }
    ctx->user_info = res->json;
  }
  return res;
}

/*
 *  get_quota of used/total
 *  https://pan.baidu.com/union/doc/Cksg0s9ic
 */
http_response *bdy_quota() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  char *url = build_url2(BDY_API_BASE, "/api/quota", "check_free", "1",
                         "checkexpire", "1", "access_token", baidu_token, NULL);
  http_request req = {.url = url, .json = true, .method = GET};
  http_response *res = malloc(sizeof(http_response));
  memset(res, 0, sizeof(http_response));
  int code = curl_request(&req, res);
  if (is_http_ok(code)) {
    XLOG(DEBUG, "quota: %s\n", json_object_to_json_string(res->json));
  }
  clean_request(&req);
  return res;
}
http_response *get_api(http_request *req) {
  http_response *p_res = malloc(sizeof(http_response));
  memset(p_res, 0, sizeof(http_response));
  int code = curl_request(req, p_res);
  if (is_http_ok(code)) {
    if (p_res->json) {
      XLOG(DEBUG, "%s\n", json_object_to_json_string(p_res->json));
    }
  }
  clean_request(req);
  return p_res;
}

http_response *bdy_file_list() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  const char *dir = "/";
  char *url = build_url2(BDY_API_BASE, "/rest/2.0/xpan/file", "method", "list",
                         "access_token", baidu_token, "dir", dir, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}

http_response *bdy_list_doc() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  const char *dir = "/";
  char *url =
      build_url2(BDY_API_BASE, "/rest/2.0/xpan/file", "method", "doclist",
                 "access_token", baidu_token, "dir", dir, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}

http_response *bdy_list_img() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  const char *dir = "/";
  char *url =
      build_url2(BDY_API_BASE, "/rest/2.0/xpan/file", "method", "imagelist",
                 "access_token", baidu_token, "dir", dir, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}
http_response *bdy_list_video() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  const char *dir = "/";
  char *url =
      build_url2(BDY_API_BASE, "/rest/2.0/xpan/file", "method", "videolist",
                 "access_token", baidu_token, "dir", dir, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}

http_response *bdy_list_bt() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  const char *dir = "/";
  char *url =
      build_url2(BDY_API_BASE, "/rest/2.0/xpan/file", "method", "btlist",
                 "access_token", baidu_token, "dir", dir, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}

http_response *bdy_category_info() {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  // const char *category = "1";
  char *url =
      build_url2(BDY_API_BASE, "/api/categoryinfo", //"category", category,
                 "access_token", baidu_token, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}

http_response *bdy_search(const char *search) {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  char *url =
      build_url2(BDY_API_BASE, "/rest/2.0/xpan/file", "method", "search", "key",
                 search, "num", "1", "access_token", baidu_token, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}
http_response *bdy_meta(int64_t fid, int32_t dlink) {
  global_ctx *ctx = get_global_ctx();
  const char *baidu_token = ctx->config->baidu_token;
  char fids[128] = {0};
  sprintf(fids, "[%lld]", fid);
  char *url = build_url2(BDY_API_BASE, "/rest/2.0/xpan/multimedia", "method",
                         "filemetas", "fsids", fids, "dlink", dlink ? "1" : "0",
                         "access_token", baidu_token, NULL);
  return get_api(&(http_request){.url = url, .json = true, .method = GET});
}

http_response *bdy_download(const char *dlink, int64_t size,
                            const char *down_local_path) {
  global_ctx *ctx = get_global_ctx();
  char *url = malloc(4096);
  memset(url, 0, 2048);
  strcat(url, dlink);
  strcat(url, "&access_token=");
  strcat(url, ctx->config->baidu_token);
  XLOG(DEBUG, "down url: %s\n", url);

  http_request req = {.url = url,
                      .json = false,
                      .method = GET,
                      .mode = MODE_DOWNLOAD,
                      .file_path = down_local_path,
                      .down_size = size};
  add_header(&req, "Host: d.pcs.baidu.com");
  return get_api(&req);
}
