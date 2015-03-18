#include "../fe.h"

int fe_is_fd_target(int fd) {
    int i;

    /* Stdin/out/err are not considered */
    if (fd < 3)
	return 0;

    /* Match in our target list */
    for (i = 0; i < fectx()->ntargets; i++)
	if (fectx()->targets[i].fd == fd) 
	    return 1;
    return 0;
}
