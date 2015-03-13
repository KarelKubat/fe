#include "../fe.h"

int is_fd_target(int fd) {
    int i;

    for (i = 0; i < fectx()->ntargets; i++)
	if (fectx()->targets[i].fd == fd) {
	    msg(fectx(), "Fd %d is a target\n", fd);
	    return 1;
	}

    msg(fectx(), "Fd %d is not a target\n", fd);
    return 0;
}
