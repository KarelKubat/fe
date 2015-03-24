#include "../fe.h"

void fe_target_add(FeCtx *ctx, char const *t) {
    struct stat statbuf;
    int statres;

    statres = stat(t, &statbuf);

    /* Only consider true files: stat must work, and must be a true file */
    if ( (!statres && fe_isfile(&statbuf)) ||
	 (statres) ) {
	ctx->targets = fe_xrealloc(ctx->targets,
				(ctx->ntargets + 1) * sizeof(Target));
	ctx->targets[ctx->ntargets].name = fe_xstrdup(t);
	ctx->targets[ctx->ntargets].fd = -1;
	ctx->ntargets++;
    }

    /* Poke new or pre-existing context into environment */
    setenv("FE_CTX", fectx_serialize(ctx), 1);
}
