#include "../fe.h"

char *xrealpath(char const *path) {
    char *ret = realpath(path, 0);
    if (ret)
	return ret;
    return xstrdup(path);
}
