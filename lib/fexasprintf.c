#include "../fe.h"

int fe_xasprintf(char **bufp, const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vasprintf(bufp, fmt, args);
    va_end(args);

    if (ret < 0)
	fe_error("Out of memory\n");

    return ret;
}
    
    
