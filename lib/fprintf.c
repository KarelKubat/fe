#include "../fe.h"

int fprintf(FILE *f, char const *fmt, ...) {
    va_list args;
    int ret;
    
    fe_msg(fectx(), "Request to fprintf(fd=%d, %s, ...)\n", fileno(f), fmt);
    va_start(args, fmt);
    ret = vfprintf(f, fmt, args);
    va_end(args);
    return ret;
}
