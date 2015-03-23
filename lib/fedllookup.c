#include "../fe.h"

void *fe_dllookup(char const *name) {
    char *dlmsg;
    void *fun;

#if 0    
    fe_msg(fectx(), "Resolving libc function %s\n", name);
#endif
    
    fun = dlsym(RTLD_NEXT, name);
    if ( (dlmsg = dlerror()) )
	fe_error("Failed to lookup libc function %s: %s\n", name, dlmsg);
    return fun;
}
    
