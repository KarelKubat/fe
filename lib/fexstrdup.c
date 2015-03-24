#include "../fe.h"

char *fe_xstrdup(char const *s) {
    char *n;

    n = strdup(s ? s : "");
    if (! n)
	fe_error("Out of memory\n");
    return n;
}
