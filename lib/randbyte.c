#include "../fe.h"

char randbyte(FeCtx *ctx, uint32_t x) {
    return randbyte_keyed(ctx->seed, x);
}
