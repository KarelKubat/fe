#include "../fe.h"

char *fe_xdirname(char const *path) {
    /* Strdup the path, dirname chops it up. The caller is responsible for
     * freeing the return value. */
    char *copy = fe_xstrdup(path);
    char *dirn = dirname(copy);
    char *ret  = fe_xstrdup(dirn);
    
    free(copy);
    return ret;
}
