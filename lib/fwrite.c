#include "../fe.h"

size_t fwrite(void const *buf, size_t sz, size_t items, FILE *f) {
    static size_t (*real_fwrite)(void const *buf, size_t sz,
				 size_t items, FILE *f);
    void *crypted_buf;
    off_t startoff;
    BitSequence hashval[HASH_BYTE_SIZE];
    size_t ret;

    if (! real_fwrite)
	real_fwrite = fe_dllookup("fwrite");

    fe_msg(fectx(), "Request to fwrite(..., %ld, %ld, fd=%d)\n",
	   (long)sz, (long)items, fileno(f));

    if (! (fe_is_fd_target(fileno(f))) )
	return real_fwrite(buf, sz, items, f);

    /* Get a copy of the buffer */
    crypted_buf = fe_xmalloc(sz * items);
    memcpy(crypted_buf, buf, sz * items);

    /* Encrypt our copy and write it. Make sure transcryption rehashes. */
    startoff = (off_t)ftell(f);
    *hashval = 0;
    fe_cryptbuf(crypted_buf, sz * items, startoff, hashval);
    ret = real_fwrite(crypted_buf, sz, items, f);

    /* Cleanup */
    free(crypted_buf);

    return ret;
}
    
