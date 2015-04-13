#include "../../fe.h"

int dup2(int fd1, int fd2) {
    static int (*real_dup2)(int fd1, int fd2);
    int ret;

    /* Get hook */
    if (! real_dup2)
	real_dup2 = fe_dllookup("dup2");

    fe_msg(fectx(), "Request to dup2(%d, %d)\n", fd1, fd2);\
    ret = real_dup2(fd1, fd2);
    fe_msg(fectx(), "Fd %d duplicated (dup2) to %d\n", fd1, ret);
    fe_target_by_fd(fd1, ret);
    return ret;
}
