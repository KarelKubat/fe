#include "../fe.h"

char *fectx_serialize(FeCtx const *ctx) {
    char *ret = 0, *cp;
    int i;

    /* Verbosity and destination of verbose messages */
    if (ctx->msg_verbosity)
	ret = xstrdup("1");
    else
	ret = xstrdup("0");

    if (ctx->msg_dst == dst_syslog)
	ret = xstrcat(ret, "l");
    else
	ret = xstrcat(ret, "e");

    /* File targets */
    xasprintf(&cp, "%d:", ctx->ntargets);
    ret = xstrcat(ret, cp);
    free(cp);
    
    for (i = 0; i < ctx->ntargets; i++) {
	ret = xstrcat(ret, ctx->targets[i].name);
	ret = xstrcat(ret, ":");
    }

    /* Random generator seed */
    ret = xstrcat(ret, seed_serialize(ctx->seed));

    /* Done */
    return ret;
}
	
		 
