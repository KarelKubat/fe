#include "../fe.h"

FeCtx *fe_setup(char const *key, int verbosity, MsgDst dst,
		int ignore_uncaught, int use_env, int debug) {
    static FeCtx ctx;

    memset(&ctx, 0, sizeof(ctx));

    ctx.seed = fe_xstrdup(key);
    ctx.msg_verbosity = verbosity;
    ctx.msg_dst = dst;
    ctx.ignore_noncaught = ignore_uncaught;
    ctx.use_env = use_env;
    ctx.debug = debug;

    fectx_set(&ctx);

    return &ctx;
}
