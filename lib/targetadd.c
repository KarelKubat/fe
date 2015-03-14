#include "../fe.h"

void target_add(FeCtx *ctx, char const *t) {
    struct stat statbuf;
    
    if (!stat(t, &statbuf) && isfile(&statbuf)) {
	ctx->targets = xrealloc(ctx->targets,
				(ctx->ntargets + 1) * sizeof(Target));
	ctx->targets[ctx->ntargets].name = xstrdup(t);
	ctx->targets[ctx->ntargets].st_dev = statbuf.st_dev;
	ctx->targets[ctx->ntargets].st_ino = statbuf.st_ino;
	ctx->targets[ctx->ntargets].fd = -1;
	ctx->ntargets++;
    }
}
