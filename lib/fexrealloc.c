#include "../fe.h"

void *fe_xrealloc(void *mem, size_t newsz) {
    if (! (mem = realloc(mem, newsz)) )
	fe_error("Out of memory\n");
    return mem;
}
