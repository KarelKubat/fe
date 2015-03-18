#include "../fe.h"

char fe_randbyte(FeCtx *ctx, uint32_t x, BitSequence *hashval) {
    return fe_randbyte_keyed(ctx->seed, x, hashval);
}
