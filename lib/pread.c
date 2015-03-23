#include "../fe.h"

ssize_t pread(int fd, void *buf, size_t bytes, off_t offset) {
    static ssize_t (*real_pread)(int fd, void *buf, size_t bytes, off_t offset);
    ssize_t ret;
    BitSequence hashval[HASH_BYTE_SIZE];

    fe_msg(fectx(), "Request to pread(%d, ..., %lu, %lld)\n",
	   fd, bytes, offset);

    if (! real_pread)
	real_pread = fe_dllookup("pread");

    if (! fe_is_fd_target(fd))
	return real_pread(fd, buf, bytes, offset);

    /* Fetch buffer and transcrypt it. Make sure transcryption rehashes. */
    ret = real_pread(fd, buf, bytes, offset);
    *hashval = 0;
    fe_cryptbuf(buf, ret, offset, hashval);
    
    return ret;    
}
