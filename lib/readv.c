#include "../fe.h"

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
    static ssize_t (*real_readv)(int fd, const struct iovec *iov, int iovcnt);
    off_t startoff;
    ssize_t ret;
    int i;

    if (! real_readv)
	real_readv = dllookup("readv");
    if (! is_fd_target(fd))
	return real_readv(fd, iov, iovcnt);

    /* Read buffers and transcrypt them */
    startoff = lseek(fd, 0, SEEK_SET);
    ret = real_readv(fd, iov, iovcnt);
    for (i = 0; i < iovcnt; i++)
	cryptbuf(iov[i].iov_base, iov[i].iov_len, startoff);

    return ret;
}
