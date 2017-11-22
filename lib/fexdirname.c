#include "../fe.h"

char *fe_xdirname(char const *path) {
    /* Strdup the path, dirname chops it up. The caller is responsible for
     * freeing the return value. */
    return dirname(fe_xstrdup(path));
}
