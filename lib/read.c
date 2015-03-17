#include "../fe.h"

ssize_t read(int fd, void *buf, size_t bytes) {
    static ssize_t (*real_read)(int fd, void *buf, size_t bytes);
    ssize_t ret;
    off_t startoff;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_read)
	real_read = dllookup("read");
    if (! is_fd_target(fd))
	return real_read(fd, buf, bytes);

    /* Fetch buffer and transcrypt it. Make sure transcryption rehashes. */
    startoff = lseek(fd, 0, SEEK_CUR);
    ret = real_read(fd, buf, bytes);
    *hashval = 0;
    cryptbuf(buf, ret, startoff, hashval);
    
    return ret;
}
