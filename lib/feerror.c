#include "../fe.h"

void fe_error(char const *fmt, ...) {
    va_list args;
    int (*real_vfprintf)(FILE *f, char const *fmt, va_list args);

    /* Don't use vfprintf here, the error might be triggered before
     * the fe context is set up */
    real_vfprintf = fe_dllookup("vfprintf");
    va_start(args, fmt);
    real_vfprintf(stderr, fmt, args);
    exit(1);
}
