#include "../fe.h"

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
    static ssize_t (*real_readv)(int fd, const struct iovec *iov, int iovcnt);

    if (! real_readv)
	real_readv = dllookup("readv");

    return real_readv(fd, iov, iovcnt);
}
