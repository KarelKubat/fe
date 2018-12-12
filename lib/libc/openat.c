#include "../../fe.h"

int openat(int f, char const *path, int oflag, ...) {
    static int (*real_openat)(int fd, char const *path, int oflag, ...);
    int mode, fd;
    va_list args;

    /* Get mode arg. According to the manpage this is only required during
     * creation, but we pass it anyway. Let the real openat() figure out if
     * it is needed. */
    va_start(args, oflag);
    mode = va_arg(args, int);
    va_end(args);
    
    /* Get hook to real openat */
    if (!real_openat)
	real_openat = fe_dllookup("openat");

    /* Perform the open */
    fd = real_openat(f, path, oflag, mode);
    fe_msg(fectx(), "Request to openat(%d, %s, %d, ...) => %d\n",
	   f, path, oflag, fd);
    fe_target_by_path(path, fd);

    return fd;
}
