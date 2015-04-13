#include "../../fe.h"

FILE *fopen(char const *path, char const *mode) {
    FILE *f;
    static FILE* (*real_fopen)(char const *path, char const *mode);

    if (! real_fopen)
	real_fopen = fe_dllookup("fopen");

    fe_msg(fectx(), "Request to fopen(%s, %s)\n", path, mode);
    if ( (f = real_fopen(path, mode)) )
	fe_target_by_path(path, fileno(f));

    return f;
}
	
