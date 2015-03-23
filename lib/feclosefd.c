#include "../fe.h"

void fe_close_fd(int fd) {
    int i;

    for (i = 0; i < fectx()->ntargets; i++)
	if (fectx()->targets[i].fd == fd) {
	    fe_msg(fectx(), "Target fd %d closed, removed from target list\n",
		fd);
	    fectx()->targets[i].fd = -1;
	    return;
	}
}
