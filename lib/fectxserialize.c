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
    asprintf(&cp, "%d:", ctx->ntargets);
    ret = xstrcat(ret, cp);
    free(cp);
    
    for (i = 0; i < ctx->ntargets; i++) {
	asprintf(&cp, "%s:%d:%llu:",
		 ctx->targets[i].name, ctx->targets[i].st_dev,
		 ctx->targets[i].st_ino);
	ret = xstrcat(ret, cp);
	free(cp);
    }

    /* Random generator seed */
    asprintf(&cp, "%u", (unsigned)ctx->seed);
    ret = xstrcat(ret, cp);
    free(cp);

    /* Done */
    return ret;
}
	
		 
