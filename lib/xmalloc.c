#include "../fe.h"

void *xmalloc(size_t sz) {
    void *ret;

    if (! (ret = calloc(sz, 1)) )
	error("Out of memory\n");
    return ret;
}
