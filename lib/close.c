#include "../fe.h"

int close(int fd) {
    static int (*real_close)(int fd);
    int ret, i;

    if (! real_close)
	real_close = fe_dllookup("close");
    
    if (! fe_is_fd_target(fd))
	return real_close(fd);

    ret = real_close(fd);
    for (i = 0; i < fectx()->ntargets; i++)
	if (fectx()->targets[i].fd == fd) {
	    fe_msg(fectx(), "Target fd %d closed, removed from target list\n",
		fd);
	    fectx()->targets[i].fd = -1;
	    return ret;
	}
    return ret;
}
