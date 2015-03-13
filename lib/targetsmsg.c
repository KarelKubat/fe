#include "../fe.h"

void targets_msg(FeCtx *ctx) {
    int i;

    if (! ctx->msg_verbosity)
	return;
    for (i = 0; i < ctx->ntargets; i++)
	msg(ctx, "Transcryption target '%s' on device %d, inode %llu\n",
	    ctx->targets[i].name, ctx->targets[i].st_dev,
	    ctx->targets[i].st_ino);
}
