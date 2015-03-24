#include "../fe.h"

int vfprintf(FILE *f, char const *fmt, va_list args) {
    static int (*real_vfprintf)(FILE *f, char const *fmt, va_list args);

    if (fe_is_fd_target(fileno(f)) && ! fectx()->ignore_noncaught)
	fe_error("fe " VER ": cannot handle vfprintf, aborting\n");

    fe_msg(fectx(), "Request to vfprintf(fd=%d, %s, ...)\n", fileno(f), fmt);
    if (! real_vfprintf)
	real_vfprintf = fe_dllookup("vfprintf");
    return real_vfprintf(f, fmt, args);
}
    
