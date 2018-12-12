#include "../fe.h"

char *fe_xbasename(char const *path) {
  /* Strdup the path, basename chops it up. The caller must free the return
   * value. */
    char *copy = fe_xstrdup(path);
    char *base = basename(copy);
    char *ret  = fe_xstrdup(base);

    free(copy);
    return ret;
}
