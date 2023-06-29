#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 哈希函数
int hashmap_hash(HashMap *hm, const char *key) {
  unsigned int hash = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
    hash = key[i] + (hash * 31);
  }
  return hash % hm->size;
}

HashNode *hashmap_create_node(const char *key, void *value) {
  HashNode *newHashNode = (HashNode *)malloc(sizeof(HashNode));
  memset(newHashNode, 0, sizeof(HashNode));
  newHashNode->key = strdup(key);
  newHashNode->value = value;
  newHashNode->next = NULL;
  return newHashNode;
}

// 初始化哈希表
HashMap *hashmap_init(HashParams params) {
  HashMap *hashMap = (HashMap *)malloc(sizeof(HashMap));
  memset(hashMap, 0, sizeof(HashMap));
  int init_size = TABLE_SIZE;
  if (params.init_size > 0) {
    init_size = params.init_size;
  }
  hashMap->size = init_size;
  hashMap->table = (HashNode **)malloc(sizeof(HashNode *) * init_size);
  memset(hashMap->table, 0, sizeof(HashNode *) * init_size);
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashMap->table[i] = NULL;
  }
  return hashMap;
}

void hashmap_insert(HashMap *hashMap, char *key, void *value) {
  int index = hashmap_hash(hashMap, key);
  HashNode *newHashNode = hashmap_create_node(key, value);

  if (hashMap->table[index] == NULL) {
    hashMap->table[index] = newHashNode;
  } else {
    HashNode *current = hashMap->table[index];
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newHashNode;
  }
}

// 获取键对应的值
void *hashmap_get(HashMap *hashMap, const char *key) {
  int index = hashmap_hash(hashMap, key);
  HashNode *current = hashMap->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      return current->value;
    }
    current = current->next;
  }
  return NULL;
}

// 更新键对应的值
void hashmap_update(HashMap *hashMap, char *key, void *value) {
  int index = hashmap_hash(hashMap, key);

  HashNode *current = hashMap->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      current->value = value;
      return;
    }
    current = current->next;
  }
}

// 删除键值对
void hashmap_remove_key(HashMap *hashMap, char *key) {
  int index = hashmap_hash(hashMap, key);

  HashNode *current = hashMap->table[index];
  HashNode *prev = NULL;

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      if (prev == NULL) {
        hashMap->table[index] = current->next;
      } else {
        prev->next = current->next;
      }
      free(current->key);
      if (current->value) {
        free(current->value);
      }
      free(current);
      return;
    }
    prev = current;
    current = current->next;
  }
}

// 清空哈希表
void hashmap_clear(HashMap *hashMap) {
  if (hashMap == NULL) {
    return;
  }
  for (int i = 0; i < hashMap->size; i++) {
    HashNode *current = hashMap->table[i];
    while (current != NULL) {
      HashNode *temp = current;
      current = current->next;
      free(temp->key);
      if (temp->value) {
        free(temp->value);
      }
      free(temp);
    }
    hashMap->table[i] = NULL;
  }
  free(hashMap->table);
  free(hashMap);
}

int main() {
  HashParams params = {TABLE_SIZE};
  HashMap *hashMap = hashmap_init(params);

  hashmap_insert(hashMap, "key1", strdup("1"));
  hashmap_insert(hashMap, "key2", strdup("2"));
  hashmap_insert(hashMap, "key3", strdup("3"));

  printf("Value for key1: %s\n", (const char *)hashmap_get(hashMap, "key1"));
  printf("Value for key2: %s\n", (const char *)hashmap_get(hashMap, "key2"));
  printf("Value for key3: %s\n", (const char *)hashmap_get(hashMap, "key3"));
  hashmap_update(hashMap, "key2", strdup("4"));
  printf("Updated value for key2: %s\n",
         (const char *)hashmap_get(hashMap, "key2"));

  hashmap_remove_key(hashMap, "key1");
  void *c = hashmap_get(hashMap, "key1");
  if (!c) {
    printf("key1 has been removed\n");
  }

  hashmap_clear(hashMap);

  return 0;
}
