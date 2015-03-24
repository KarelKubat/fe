#include "../fe.h"

size_t fread(void *buf, size_t sz, size_t items, FILE *f) {
    static size_t (*real_fread)(void *buf, size_t sz, size_t items, FILE *f);
    size_t ret;
    off_t startoff;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_fread)
	real_fread = fe_dllookup("fread");
    if (! fe_is_fd_target(fileno(f)) )
	return real_fread(buf, sz, items, f);

    fe_msg(fectx(), "Request to fread(..., %ld, %ld, fd=%d)\n",
	   (long)sz, (long)items, fileno(f));

    startoff = (off_t)ftell(f);
    *hashval = 0;
    ret = real_fread(buf, sz, items, f);
    fe_cryptbuf(buf, sz * items, startoff, hashval);

    return ret;
}
