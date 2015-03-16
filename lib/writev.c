#include "../fe.h"

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
    static ssize_t (*real_writev)(int fd, const struct iovec *iov, int iovcnt);
    off_t startoff;
    int i;

    if (! real_writev)
	real_writev = dllookup("writev");
    if (! is_fd_target(fd))
	return real_writev(fd, iov, iovcnt);

    /* Transcrypt buffers and write */
    startoff = lseek(fd, 0, SEEK_SET);
    for (i = 0; i < iovcnt; i++)
	cryptbuf(iov[i].iov_base, iov[i].iov_len, startoff);
    
    return real_writev(fd, iov, iovcnt);
}
