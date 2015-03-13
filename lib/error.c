#include "../fe.h"

void error(char const *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    exit(1);
}
