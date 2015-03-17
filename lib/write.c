#include "../fe.h"

ssize_t write(int fd, const void *buf, size_t bytes) {
    static ssize_t (*real_write)(int fd, const void *buf, size_t bytes);
    ssize_t ret;
    off_t startoff;
    void *crypted_buf;
    BitSequence hashval[HASH_BYTE_SIZE];

    if (! real_write)
	real_write = dllookup("write");

    if (! is_fd_target(fd))
	return real_write(fd, buf, bytes);

    /* Get copy of the buffer */
    crypted_buf = xmalloc(bytes);
    memcpy(crypted_buf, buf, bytes);

    /* Encrypt our copy and write it. Make sure transcryption rehashes. */
    startoff = lseek(fd, 0, SEEK_CUR);
    *hashval = 0;
    cryptbuf(crypted_buf, bytes, startoff, hashval);
    ret = real_write(fd, crypted_buf, bytes);

    /* Cleanup */
    free(crypted_buf);

    return ret;
}
