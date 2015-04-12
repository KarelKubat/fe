#include "../fe.h"

int fputs(char const *s, FILE *f) {
    static int (*real_fputs)(char const *s, FILE *f);

    if (! real_fputs)
	real_fputs = fe_dllookup("fputs");
    if (! fe_is_fd_target(fileno(f)) )
	return real_fputs(s, f);

    /* Dump out buffer using fwrite() */
    return fwrite(s, 1, strlen(s), f);
}
