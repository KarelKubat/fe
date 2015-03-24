#include "../fe.h"

char *fectx_serialize(FeCtx const *ctx) {
    char *ret = 0, *cp;
    int i;

    /* Verbosity and destination of verbose messages */
    if (ctx->msg_verbosity)
	ret = fe_xstrdup("1");
    else
	ret = fe_xstrdup("0");

    if (ctx->msg_dst == dst_syslog)
	ret = fe_xstrcat(ret, "l");
    else
	ret = fe_xstrcat(ret, "e");

    if (ctx->ignore_noncaught)
	ret = fe_xstrcat(ret, "1");
    else
	ret = fe_xstrcat(ret, "0");

    /* File targets */
    fe_xasprintf(&cp, "%d:", ctx->ntargets);
    ret = fe_xstrcat(ret, cp);
    free(cp);
    
    for (i = 0; i < ctx->ntargets; i++) {
	ret = fe_xstrcat(ret, ctx->targets[i].name);
	ret = fe_xstrcat(ret, ":");
    }

    /* Random generator seed */
    ret = fe_xstrcat(ret, fe_seed_serialize(ctx->seed));

    /* Done */
    return ret;
}
	
		 
