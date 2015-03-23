#include "../fe.h"

int close(int fd) {
    static int (*real_close)(int fd);
    int ret;

    fe_msg(fectx(), "Request to close(%d)\n", fd);

    if (! real_close)
	real_close = fe_dllookup("close");
    
    if (! fe_is_fd_target(fd))
	return real_close(fd);

    ret = real_close(fd);
    fe_close_fd(fd);
    return ret;
}
