#include "../fe.h"

char *fe_xbasename(char const *path) {
    char *cp;

    if ( (cp = strchr(path, '/')) )
	return fe_xstrdup(cp + 1);
    return fe_xstrdup(path);
}
