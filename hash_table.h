#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stddef.h>
#define TABLE_SIZE 100

typedef struct HashNode {
  char *key;
  void *value;
  struct HashNode *next;
} HashNode;

typedef struct HashMap {
  HashNode **table;
  size_t size;
} HashMap;

typedef struct HashParams {
  size_t init_size;
} HashParams;

// init
HashMap *hashmap_init(HashParams params);

// hash key->int
int hashmap_hash(HashMap *hm, const char *key);

// 创建一个新的节点
HashNode *hashmap_create_node(const char *key, void *pvalue);
// 初始化哈希表

// 插入键值对到哈希表
void hashmap_insert(HashMap *hashMap, char *key, void *value);

// 获取键对应的值
void *hashmap_get(HashMap *hashMap, const char *key);

// 更新键对应的值
void hashmap_update(HashMap *hashMap, char *key, void *value);
// 删除键值对
void hashmap_remove_key(HashMap *hashMap, char *key);

// 清空哈希表
void hashmap_clear(HashMap *hashMap);

#endif
