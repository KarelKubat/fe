#include "../fe.h"

char *fe_xrealpath(char const *path) {
    char *ret = realpath(path, 0);
    if (ret)
	return ret;
    return fe_xstrdup(path);
}
