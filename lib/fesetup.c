#include "../fe.h"

FeCtx *fe_setup(char const *key, int verbosity, MsgDst dst,
		int ignore_uncaught, int use_env) {
    static FeCtx ctx;

    memset(&ctx, 0, sizeof(ctx));

    ctx.seed = fe_xstrdup(key);
    ctx.msg_verbosity = verbosity;
    ctx.msg_dst = dst;
    ctx.ignore_noncaught = ignore_uncaught;
    ctx.use_env = use_env;
    
    printf("use env: %d\n", use_env);

    fectx_set(&ctx);

    return &ctx;
}
