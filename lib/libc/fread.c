#include "../../fe.h"

size_t fread(void *buf, size_t sz, size_t items, FILE *f) {
    static size_t (*real_fread)(void *buf, size_t sz, size_t items, FILE *f);
    size_t ret;
    off_t startoff;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_fread)
	real_fread = fe_dllookup("fread");
    if (! fe_is_fd_target(fileno(f)) )
	return real_fread(buf, sz, items, f);

    startoff = (off_t)ftell(f);
    *hashval = 0;
    ret = real_fread(buf, sz, items, f);
    fe_msg(fectx(), "Request to fread(buf, %ld, %ld, fd=%d) "
	   "from offset %lu => %lu\n",
	   (long)sz, (long)items, fileno(f), (long)startoff, ret);
    fe_cryptbuf(buf, sz * ret, startoff, hashval);

    return ret;
}
