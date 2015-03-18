#include "../fe.h"

int open(char const *path, int oflag, ...) {
    static int (*real_open)(char const *path, int oflag, ...);
    va_list args;
    int mode, fd;

    /* Get mode arg */
    va_start(args, oflag);
    mode = va_arg(args, int);
    va_end(args);

    /* Get hook to original open */
    if (!real_open)
	real_open = fe_dllookup("open");

    /* Open file, update fd if it's a target */
    fd = real_open(path, oflag, mode);
    fe_target_update_fd(path, fd);
    return fd;
}
