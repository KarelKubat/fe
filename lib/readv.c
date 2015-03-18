#include "../fe.h"

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
    static ssize_t (*real_readv)(int fd, const struct iovec *iov, int iovcnt);
    off_t startoff;
    ssize_t ret;
    int i;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_readv)
	real_readv = fe_dllookup("readv");
    if (! fe_is_fd_target(fd))
	return real_readv(fd, iov, iovcnt);

    /* Read buffers and transcrypt them. Make sure transcryption rehashes. */
    startoff = lseek(fd, 0, SEEK_SET);
    ret = real_readv(fd, iov, iovcnt);
    *hashval = 0;
    for (i = 0; i < iovcnt; i++) 
	fe_cryptbuf(iov[i].iov_base, iov[i].iov_len, startoff, hashval);

    return ret;
}
