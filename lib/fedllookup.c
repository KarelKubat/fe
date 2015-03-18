#include "../fe.h"

void *fe_dllookup(char const *name) {
    char *dlmsg;
    void *fun;

    fun = dlsym(RTLD_NEXT, name);
    if ( (dlmsg = dlerror()) )
	fe_error("Failed to lookup libc function %s: %s\n", name, dlmsg);
    return fun;
}
    
