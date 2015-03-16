#include "../fe.h"

void target_add(FeCtx *ctx, char const *t) {
    struct stat statbuf;
    int statres;

    statres = stat(t, &statbuf);
    if ( (!statres && isfile(&statbuf)) ||
	 (statres) ) {
	ctx->targets = xrealloc(ctx->targets,
				(ctx->ntargets + 1) * sizeof(Target));
	ctx->targets[ctx->ntargets].name = xstrdup(t);
	ctx->targets[ctx->ntargets].fd = -1;
	ctx->ntargets++;
    }
}
