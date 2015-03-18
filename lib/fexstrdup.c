#include "../fe.h"

char *fe_xstrdup(char const *s) {
    char *n;
    
    if (! (n = strdup(s)) )
	fe_error("Out of memory\n");
    return n;
}
