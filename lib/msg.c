#include "../fe.h"

void msg(FeCtx const *ctx, char const *fmt, ...) {
    va_list args;

    if (! ctx->msg_verbosity)
	return;

    va_start(args, fmt);
    if (ctx->msg_dst == dst_stderr)
	vfprintf(stderr, fmt, args);
    else if (ctx->msg_dst == dst_syslog) {
	static int log_initialized;

	if (! log_initialized) {
	    openlog("fe", LOG_PID, LOG_USER);
	    log_initialized++;
	}
	vsyslog(LOG_WARNING, fmt, args);
    }
    va_end(args);
}
