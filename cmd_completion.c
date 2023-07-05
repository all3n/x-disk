#include "cmd_completion.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *commands[] = {"help", "quit", "exit", "list", NULL};
// 自定义的回调函数，用于生成tab提示
char *command_generator(const char *text, int state) {
  static int list_index, len;
  char *name;

  // 如果是第一次调用，则初始化索引和字符串长度
  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  // 遍历命令数组，查找以text开头的命令
  while ((name = commands[list_index])) {
    list_index++;
    if (strncmp(name, text, len) == 0) {
      return strdup(name);
    }
  }

  // 如果没有找到匹配的命令，则返回NULL
  return NULL;
}

char **command_completion(const char *text, int start, int end) {
  char **matches = NULL;

  // 如果是第一个参数，则使用command_generator生成提示
  if (start == 0) {
    matches = rl_completion_matches(text, command_generator);
  }

  return matches;
}

static void init_readline() {
  rl_attempted_completion_function = command_completion;
  rl_bind_key('\t', rl_complete);
  using_history();
}
