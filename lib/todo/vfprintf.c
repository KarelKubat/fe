#include "../fe.h"

int vfprintf(FILE *f, char const *fmt, va_list args) {
    int ret;
    char *buf;
    static int (*real_vfprintf)(FILE *f, char const *fmt, va_list args);

    fe_msg(fectx(), "Request to vfprintf(fd=%d, %s, ...)\n", fileno(f), fmt);

    if (! real_vfprintf)
	real_vfprintf = fe_dllookup("vfprintf");
    if (! fe_is_fd_target(fileno(f)) )
	return real_vfprintf(f, fmt, args);
    
    fe_xvasprintf(&buf, fmt, args);
    ret = write(fileno(f), buf, strlen(buf));
    free(buf);

    return ret;
}
    
