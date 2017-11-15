#include "../../fe.h"

int vfprintf(FILE *f, char const *fmt, va_list args) {
    static int (*real_vfprintf)(FILE *f, char const *fmt, va_list args);
    static size_t (*real_fwrite)(void const *buf, size_t sz,
				 size_t items, FILE *f);

    char *buf;
    BitSequence hashval[HASH_BYTE_SIZE];
    off_t startoff;
    int ret;

    if (! real_vfprintf)
	real_vfprintf = fe_dllookup("vfprintf");
    if (! real_fwrite)
	real_fwrite = fe_dllookup("fwrite");
    if (! fe_is_fd_target(fileno(f)) )
	return real_vfprintf(f, fmt, args);

    fe_msg(fectx(), "Request to vfprintf(fd=%d, %s, args)\n", fileno(f), fmt);
    startoff = ftell(f);
    *hashval = 0;
    ret = fe_xvasprintf(&buf, fmt, args);
    fe_cryptbuf(buf, ret, startoff, hashval);
    real_fwrite(buf, 1, ret, f);
    return ret;
}
