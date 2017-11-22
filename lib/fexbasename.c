#include "../fe.h"

char *fe_xbasename(char const *path) {
  /* Strdup the path, basename chops it up. The caller must free the return
   * value. */
  return basename(fe_xstrdup(path));
}
