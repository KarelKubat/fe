#include "../fe.h"

int fclose(FILE *f) {
    static int (*real_fclose)(FILE *f);
    int ret;

    fe_msg(fectx(), "Request to fclose(fd=%d)\n", fileno(f));
    
    if (! real_fclose)
	real_fclose = fe_dllookup("fclose");

    if (! fe_is_fd_target(fileno(f)) )
	return real_fclose(f);

    fe_close_fd(fileno(f));
    ret = real_fclose(f);
    return ret;
}
