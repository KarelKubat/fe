#include "../fe.h"

ssize_t read(int fd, void *buf, size_t bytes) {
    static ssize_t (*real_read)(int fd, void *buf, size_t bytes);
    ssize_t ret;
    off_t startoff;

    if (! real_read)
	real_read = dllookup("read");

    if (! is_fd_target(fd))
	return real_read(fd, buf, bytes);

    /* Fetch buffer and transcrypt it */
    startoff = lseek(fd, 0, SEEK_CUR);
    ret = real_read(fd, buf, bytes);
    cryptbuf(buf, ret, startoff);
    
    return ret;
}
