#include "../fe.h"

void fe_error(char const *fmt, ...) {
    va_list args;
    int (*real_vfprintf)(FILE *f, char const *fmt, va_list args);
    int (*real_fprintf)(FILE *f, char const *fmt, ...);

    /* Don't use vfprintf here, the error might be triggered before
     * the fe context is set up */
    real_vfprintf = fe_dllookup("vfprintf");
    real_fprintf = fe_dllookup("fprintf");
    real_fprintf(stderr, "fe [%d]: ", getpid());
    va_start(args, fmt);
    real_vfprintf(stderr, fmt, args);
    exit(1);
}
