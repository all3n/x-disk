#ifndef _FILE_IO_H
#define _FILE_IO_H

#include <stdio.h>
#include <json-c/json.h>
struct json_object * load_json_from_file(const char *path);
#endif // !#ifndef _FILE_IO_H
