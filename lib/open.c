#include "../fe.h"

int open(char const *path, int oflag, ...) {
    static int (*real_open)(char const *path, int oflag, ...);
    va_list args;
    int mode, fd;

    fe_msg(fectx(), "Request to open(%s, %d, ...)\n", path, oflag);

    /* Get mode arg */
    va_start(args, oflag);
    mode = va_arg(args, int);
    va_end(args);

    /* Get hook to original open */
    if (!real_open)
	real_open = fe_dllookup("open");

    /* Open file, update fd if it's a target */
    fd = real_open(path, oflag, mode);
    fe_target_by_path(path, fd);
    return fd;
}

int open64(char const *path, int oflag, ...) {
    static int (*real_open64)(char const *path, int oflag, ...);
    va_list args;
    int mode, fd;

    fe_msg(fectx(), "Request to open64(%s, %d, ...)\n", path, oflag);

    /* Get mode arg */
    va_start(args, oflag);
    mode = va_arg(args, int);
    va_end(args);

    /* Get hook to original open64 */
    if (!real_open64)
	real_open64 = fe_dllookup("open64");

    /* Open file, update fd if it's a target */
    fd = real_open64(path, oflag, mode);
    fe_target_by_path(path, fd);
    return fd;
}
