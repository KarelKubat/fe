#include "../../fe.h"

ssize_t pwrite(int fd, const void *buf, size_t bytes, off_t offset) {
    static ssize_t (*real_pwrite)(int fd, const void *buf, size_t bytes,
				  off_t offset);
    ssize_t ret;
    void *crypted_buf;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_pwrite)
	real_pwrite = fe_dllookup("pwrite");

    if (! fe_is_fd_target(fd))
	return real_pwrite(fd, buf, bytes, offset);

    /* Get copy of the buffer */
    crypted_buf = fe_xmalloc(bytes);
    memcpy(crypted_buf, buf, bytes);

    /* Encrypt our copy and write it. Make sure transcryption rehashes. */
    *hashval = 0;
    fe_cryptbuf(crypted_buf, bytes, offset, hashval);
    ret = real_pwrite(fd, crypted_buf, bytes, offset);

    /* Cleanup */
    free(crypted_buf);

    return ret;
}
