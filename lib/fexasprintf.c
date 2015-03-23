#include "../fe.h"

int fe_xasprintf(char **bufp, const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = fe_xvasprintf(bufp, fmt, args);
    va_end(args);

    return ret;
}
    
    
