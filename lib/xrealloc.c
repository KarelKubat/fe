#include "../fe.h"

void *xrealloc(void *mem, size_t newsz) {
    if (! (mem = realloc(mem, newsz)) )
	error("Out of memory\n");
    return mem;
}
