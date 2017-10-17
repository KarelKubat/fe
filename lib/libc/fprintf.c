#include "../../fe.h"

int fprintf(FILE *f, char const *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vfprintf(f, fmt, args);
    va_end(args);
    return ret;
}
