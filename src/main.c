#include "cmd.h"
#include "cmd_completion.h"
#include "common.h"
#include "utils.h"
#include <string.h>

// CLFAGS:
// SOURCES: curl_utils.c utils.c file_io.c common.c baidu_yun.c
// LIBS: -lcurl -ljson-c -lreadline
// BUILD_DIR: build
int main(int argc, char *argv[]) {
  init_readline();
  global_ctx *ctx = get_global_ctx();
  init_global_ctx(ctx);
  if (!ctx->config->baidu_token) {
    XLOG(WARNING, "baidu_token is null");
    return -1;
  }
  if (argc == 1 || strcmp(argv[1], "-i") == 0) {
    run_interactive();
  } else {
    run_cmd(argc, argv);
  }
  clean_global_ctx(ctx);
  return EXIT_SUCCESS;
}
