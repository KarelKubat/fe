#include "../fe.h"

void targets_msg(FeCtx *ctx) {
    int i;

    if (! ctx->msg_verbosity)
	return;
    for (i = 0; i < ctx->ntargets; i++)
	fe_msg(ctx, "Transcryption target: '%s'\n", ctx->targets[i].name);
}
