#ifndef _CURL_UTILS_H
#define _CURL_UTILS_H

#include <curl/curl.h>
#include <json-c/json.h>
#include <stdbool.h>

typedef enum http_method {
  GET = 0,
  POST,
  PUT,
  DELETE,
  HEAD,
  PATCH
} http_method;

typedef struct http_request {
  char *url;
  bool json;
  http_method method;
  const char *data;
  struct curl_slist *headers;
  const char *file_path;

} http_request;

typedef struct http_response {
  char *data;
  size_t size;
  struct json_object *json;
} http_response;

bool is_http_ok(int code);

void add_header(http_request *req, const char *header);
// 发送GET请求
int curl_request(http_request *request, http_response *response);

char *build_url(const char *base_url, ...);
char *build_url2(const char *base_url, const char *query_path, ...);
// callbacks
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);

// clean resources
void clean_request(http_request *request);
void clean_response(http_response *response);
#endif // !#ifndef _CURL_UTILS_H
