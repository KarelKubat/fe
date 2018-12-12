#include "../fe.h"

void *fe_xrealloc(void *mem, size_t newsz) {
    if (!mem)
	mem = malloc(newsz);
    else
	mem = realloc(mem, newsz);
    
    if (!mem)
	fe_error("Out of memory\n");
    return mem;
}
