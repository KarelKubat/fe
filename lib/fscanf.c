#include "../fe.h"

int fscanf(FILE *f, char const *fmt, ...) {
    va_list args;
    int ret;

    if (fe_is_fd_target(fileno(f)) && ! fectx()->ignore_noncaught)
	fe_error("fe " VER ": cannot handle fscanf, aborting\n");

    fe_msg(fectx(), "Request to fscanf(fd=%d, %s, ...)\n", fileno(f), fmt);
    va_start(args, fmt);
    ret = vfscanf(f, fmt, args);
    va_end(args);
    return ret;
}
