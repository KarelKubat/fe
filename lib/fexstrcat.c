#include "../fe.h"

char *fe_xstrcat(char *mem, char const *what) {
    if (!mem)
	return fe_xstrdup(what);
    if (!what)
	return mem;

    mem = fe_xrealloc(mem, strlen(mem) + strlen(what) + 1);
    return strcat(mem, what);
}
