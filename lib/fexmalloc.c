#include "../fe.h"

void *fe_xmalloc(size_t sz) {
    void *ret;

    if (! (ret = calloc(sz, 1)) )
	fe_error("Out of memory\n");
    return ret;
}
