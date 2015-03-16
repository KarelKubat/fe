#include "../fe.h"

int xasprintf(char **bufp, const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vasprintf(bufp, fmt, args);
    va_end(args);

    if (ret < 0)
	error("Out of memory\n");

    return ret;
}
    
    
