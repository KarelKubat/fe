#include "../fe.h"

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
    static ssize_t (*real_writev)(int fd, const struct iovec *iov, int iovcnt);

    if (! real_writev)
	real_writev = dllookup("writev");

    return real_writev(fd, iov, iovcnt);
}
