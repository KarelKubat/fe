#include "../fe.h"

int vfscanf(FILE *f, char const *fmt, va_list args) {
    static int (*real_vfscanf)(FILE *f, char const *fmt, va_list args);

    if (fe_is_fd_target(fileno(f)) && ! fectx()->ignore_noncaught)
	fe_error("fe " VER ": cannot handle vfscanf, aborting\n");

    if (! real_vfscanf)
	real_vfscanf = fe_dllookup("vfscanf");
    return real_vfscanf(f, fmt, args);
}
