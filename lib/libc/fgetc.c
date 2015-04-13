#include "../../fe.h"

int fgetc(FILE *f) {
    static int (*real_fgetc)(FILE *f);
    char buf;

    if (! real_fgetc)
	real_fgetc = fe_dllookup("fgetc");
    if (! fe_is_fd_target(fileno(f)) )
	return real_fgetc(f);

    /* Get char using fread(), return EOF if necessary */
    if (!fread(&buf, 1, 1, f))
	return EOF;
    return (int)buf;
}
    
