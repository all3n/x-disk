#include "utils.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <json-c/json_tokener.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "curl_utils.h"

bool is_http_ok(int code) { return code == CURLE_OK; }
void add_header(http_request *req, const char *header) {
  req->headers = curl_slist_append(req->headers, header);
}
// 发送GET请求
int curl_request(http_request *request, http_response *response) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, request->url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    switch (request->method) {
    case GET:
      break;
    case POST:
      curl_easy_setopt(curl, CURLOPT_POST, 1);
      if (request->data && strlen(request->data) > 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request->data);
      } else {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
      }

      if (request->file_path && strlen(request->file_path) > 0) {
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, request->file_path);
      }
      break;
    default:
      break;
    }
    if (request->headers) {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request->headers);
    }

    res = curl_easy_perform(curl);
    if (request->json && response->data) {
      response->json = json_tokener_parse(response->data);
      // free txt response
      free(response->data);
      response->data = NULL;
    }
    curl_easy_cleanup(curl);
    if (res == CURLE_OK) {
      return 0;
    } else {
      return res;
    }
  }
  return -1;
}

char *build_url(const char *base_url, ...) {
  va_list arg_list;
  va_start(arg_list, base_url);
  int total_len = strlen(base_url) + 1;
  int num_args = 0;
  while (1) {
    char *key = va_arg(arg_list, char *);
    if (key == NULL) {
      break;
    }
    char *value = va_arg(arg_list, char *);
    if (value == NULL) {
      fprintf(stderr, "build_url: key:%s value is NULL,ignore\n", key);
      break;
    }
    num_args += 1;
    total_len += strlen(key) + strlen(value) + 2;
  }
  va_end(arg_list);

  // 分配内存
  char *url = (char *)malloc(total_len);
  if (!url) {
    return NULL;
  }

  va_start(arg_list, base_url);
  // 构造URL
  strcpy(url, base_url);
  for (int i = 0; i < num_args; i++) {
    char *key = va_arg(arg_list, char *);
    char *value = va_arg(arg_list, char *);
    strcat(url, i == 0 ? "?" : "&");
    strcat(url, key);
    strcat(url, "=");
    strcat(url, value);
  }

  va_end(arg_list);
  return url;
}

char *build_url2(const char *base_url, const char *query_path, ...) {
  va_list arg_list;
  va_start(arg_list, query_path);
  int total_len = strlen(base_url) + strlen(query_path) + 1;
  int num_args = 0;
  while (1) {
    char *key = va_arg(arg_list, char *);
    if (key == NULL) {
      break;
    }
    char *value = va_arg(arg_list, char *);
    if (value == NULL) {
      fprintf(stderr, "build_url: key:%s value is NULL,ignore\n", key);
      break;
    }
    num_args += 1;
    total_len += strlen(key) + strlen(value) + 2;
  }
  va_end(arg_list);

  // 分配内存
  char *url = (char *)malloc(total_len);
  if (!url) {
    return NULL;
  }

  va_start(arg_list, query_path);
  // 构造URL
  strcpy(url, base_url);
  strcat(url, query_path);
  for (int i = 0; i < num_args; i++) {
    char *key = va_arg(arg_list, char *);
    char *value = va_arg(arg_list, char *);
    strcat(url, i == 0 ? "?" : "&");
    strcat(url, key);
    strcat(url, "=");
    strcat(url, value);
  }

  va_end(arg_list);
  XLOG("build_url: %s\n", url);
  return url;
}

// 回调函数，用于处理curl的返回结果
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
  http_response *data = (http_response *)userdata;
  size_t real_size = size * nmemb;
  data->data = (char *)realloc(data->data, data->size + real_size + 1);
  if (data->data == NULL) {
    fprintf(stderr, "realloc failed\n");
    return 0;
  }
  memcpy(data->data + data->size, ptr, real_size);
  data->size += real_size;
  data->data[data->size] = '\0';
  return real_size;
}

// 回调函数，用于上传文件
size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
  FILE *file = fopen((char *)stream, "rb");
  size_t nread;
  if (file) {
    nread = fread(ptr, size, nmemb, file);
    fclose(file);
    return nread;
  }
  return 0;
}

void clean_request(http_request *request) {
  if (request->url) {
    free(request->url);
  }
  // if(request->data){
  //   free(request->data);
  // }
  if (request->headers) {
    curl_slist_free_all(request->headers);
  }
}
void clean_response(http_response *response) {
  if (response == NULL) {
    return;
  }
  if (response->data) {
    free(response->data);
  }
  if (response->json) {
    json_object_put(response->json);
  }
  free(response);
}
