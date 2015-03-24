#include "../fe.h"

FeCtx *fe_setup(char const *key, int verbosity, MsgDst dst,
		int ignore_uncaught) {
    static FeCtx ctx;

    memset(&ctx, 0, sizeof(ctx));

    ctx.seed = fe_xstrdup(key);
    ctx.msg_verbosity = verbosity;
    ctx.msg_dst = dst;
    ctx.ignore_noncaught = ignore_uncaught;

    setenv("FE_CTX", fectx_serialize(&ctx), 1);

    return &ctx;
}
