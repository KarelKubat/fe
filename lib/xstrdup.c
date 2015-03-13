#include "../fe.h"

char *xstrdup(char const *s) {
    char *n;
    
    if (! (n = strdup(s)) )
	error("Out of memory\n");
    return n;
}
