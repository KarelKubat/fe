#include "../fe.h"

char *fe_xdirname(char const *path) {
    char *ret = fe_xstrdup(path);
    char *cp;

    if ( (cp = strrchr(ret, '/')) ) {
	*cp = 0;
	return ret;
    }
    return fe_xstrdup(".");
}
