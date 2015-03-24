#include "../fe.h"

int dup(int fd) {
    static int (*real_dup)(int fd);
    int dupfd;

    /* Get hook */
    if (! real_dup)
	real_dup = fe_dllookup("dup");

    fe_msg(fectx(), "Request to dup(%d)\n", fd);
    dupfd = real_dup(fd);
    fe_msg(fectx(), "Fd %d duplicated (dup) to %d\n", fd, dupfd);
    fe_target_by_fd(fd, dupfd);
    return dupfd;
}
