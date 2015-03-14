#include "../fe.h"

void randinit(FeCtx *ctx, char const *buf) {
    ctx->seed = xstrdup(buf);
}
