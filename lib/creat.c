#include "../fe.h"

int creat(char const *path, mode_t mode) {
    static int (*real_creat)(char const *path, mode_t mode);
    int fd;

    /* Get hook to original creat */
    if (! real_creat)
	real_creat = fe_dllookup("creat");

    /* Create the file, update fd if it's a target */
    fd = real_creat(path, mode);
    fe_target_update_fd(path, fd);
    return fd;
}
