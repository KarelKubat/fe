#include "../fe.h"

void *dllookup(char const *name) {
    char *dlmsg;
    void *fun;

    fun = dlsym(RTLD_NEXT, name);
    if ( (dlmsg = dlerror()) )
	error("Failed to lookup libc function %s: %s\n", name, dlmsg);
    msg(fectx(), "Resolved libc symbol %s\n", name);
    return fun;
}
    
