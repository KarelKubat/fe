#include "../fe.h"

int fprintf(FILE *f, char const *fmt, ...) {
    va_list args;
    int ret;

    if (fe_is_fd_target(fileno(f)) && ! fectx()->ignore_noncaught)
	fe_error("fe " VER ": cannot handle fprintf, aborting\n");
    
    fe_msg(fectx(), "Request to fprintf(fd=%d, %s, ...)\n", fileno(f), fmt);
    va_start(args, fmt);
    ret = vfprintf(f, fmt, args);
    va_end(args);
    return ret;
}
