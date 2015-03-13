#include "../fe.h"

char *xstrcat(char *mem, char const *what) {
    mem = xrealloc(mem, strlen(mem) + strlen(what) + 1);
    return strcat(mem, what);
}
