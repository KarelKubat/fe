#include "../fe.h"

ssize_t writev(int fd, const struct iovec *iovec, int iovcnt) {
    static ssize_t (*real_writev)(int fd, const struct iovec *iov, int iovcnt);
    off_t startoff;
    int i;
    struct iovec *iv;
    ssize_t ret;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_writev)
	real_writev = dllookup("writev");
    if (! is_fd_target(fd))
	return real_writev(fd, iovec, iovcnt);

    /* Remember the offset. Used to key all iovec buffer transcrypts. */    
    startoff = lseek(fd, 0, SEEK_SET);
    
    /* Make our own iovec and transcrypt. Make sure transcryption rehashes. */
    iv = xmalloc(iovcnt * sizeof(struct  iovec));
    for (i = 0; i < iovcnt; i++) {
	iv[i].iov_len  = iovec[i].iov_len;
	iv[i].iov_base = xmalloc(iv[i].iov_len);
	memcpy(iv[i].iov_base, iovec[i].iov_base, iv[i].iov_len);
	*hashval = 0;
	cryptbuf(iv[i].iov_base, iv[i].iov_len, startoff, hashval);
    }

    /* Write it out */
    ret = real_writev(fd, iv, iovcnt);

    /* Free up memory */
    for (i = 0; i < iovcnt; i++)
	free(iv[i].iov_base);
    free(iv);

    /* Done */
    return ret;
}
