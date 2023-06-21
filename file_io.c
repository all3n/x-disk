#include "file_io.h"
#include <json-c/json_types.h>

struct json_object *load_json_from_file(const char *path) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    printf("open file %s failed\n", path);
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = (char *)malloc(size + 1);
  fread(buffer, size, 1, fp);
  buffer[size] = '\0';
  struct json_object *root = json_tokener_parse(buffer);
  if (root == NULL) {
    printf("config json is null\n");
  }
  // printf("%s\n", json_object_to_json_string(root));
  free(buffer);
  fclose(fp);

  return root;
}
