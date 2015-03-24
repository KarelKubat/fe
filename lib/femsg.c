#include "../fe.h"

void fe_msg(FeCtx const *ctx, char const *fmt, ...) {
    va_list args;
    static int (*real_vfprintf)(FILE *f, char const *fmt, va_list args);
    static int log_initialized;


    if (! ctx->msg_verbosity)
	return;

    va_start(args, fmt);
    if (ctx->msg_dst == dst_stderr) {
	if (! real_vfprintf)
	    real_vfprintf = fe_dllookup("vfprintf");
	real_vfprintf(stderr, fmt, args);
    } else if (ctx->msg_dst == dst_syslog) {
	if (! log_initialized) {
	    openlog("fe", LOG_PID, LOG_USER);
	    log_initialized++;
	}
	vsyslog(LOG_WARNING, fmt, args);
    }
    va_end(args);
}
