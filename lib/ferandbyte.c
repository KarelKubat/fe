#include "../fe.h"

char fe_randbyte(FeCtx *ctx, uint32_t x, BitSequence *hashval, int algorithm) {
    return fe_randbyte_keyed(ctx->seed, x, hashval, algorithm);
}
