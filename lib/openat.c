#include "../fe.h"

int openat(int fd, char const *path, int oflag, ...) {
    fe_msg(fectx(), "Request to openat(%d, %s, %d, ...\n", fd, path, oflag);    
    fe_error("fe " VER ": cannot handle openat, aborting\n");
    return -1;
}
