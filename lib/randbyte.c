#include "../fe.h"

char randbyte(FeCtx *ctx, uint32_t x, BitSequence *hashval) {
    return randbyte_keyed(ctx->seed, x, hashval);
}
