#include "../fe.h"

char *fgets(char *buf, int sz, FILE *f) {
    static char* (*real_fgets)(char *buf, int sz, FILE *f);
    int i;
    char ch;

    if (! real_fgets)
	real_fgets = fe_dllookup("fgets");
    if (! fe_is_fd_target(fileno(f)) )
	return real_fgets(buf, sz, f);

    /* Fetch up to and including sz-1 chars using fgetc(),
     * stop on EOF or \n. 
     */
    for (i = 0; i < sz - 1; i++) {
	ch = (char)fgetc(f);
	if (ch == '\n' || ch == EOF)
	    break;
	buf[i] = ch;
    }
    /* If EOF occurs right at the start, then the buffer remains
     * unchanged and NULL is returned. Else, buffer must be \0-terminated
     * and returned. */
    if (!i)
	return 0;
    
    buf[i] = 0;
    return buf;
}
