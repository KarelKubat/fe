#include "../fe.h"

int creat(char const *path, mode_t mode) {
    static int (*real_creat)(char const *path, mode_t mode);
    int fd;

    fe_msg(fectx(), "Request to creat(%s, %d)\n", path, mode);

    /* Get hook to original creat */
    if (! real_creat)
	real_creat = fe_dllookup("creat");

    /* Create the file, update fd if it's a target */
    fd = real_creat(path, mode);
    fe_target_by_path(path, fd);
    return fd;
}
