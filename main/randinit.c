#include "../fe.h"

void randinit(FeCtx *ctx, char const *buf) {
    ctx->seed = fe_xstrdup(buf);
}
