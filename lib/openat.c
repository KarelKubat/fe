#include "../fe.h"

int openat(int fd, char const *path, int oflag, ...) {
    error("Sorry, fe V" VER " does not support openat().\n"
	  "You can't use fe with this program.\n");
    return -1;
}
