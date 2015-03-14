#include "../fe.h"

char *readkey(void) {
    static char buf[256];
    char *cp;

    /* Cannot use our own read() here, crypto isn't set up yet. */
    ssize_t (*real_read)(int fd, void *buf, size_t bytes) = dllookup("read");

    real_read(0, buf, sizeof(buf) - 1);
    if ( (cp = strchr(buf, '\n')) )
	*cp = 0;
    if (! strlen(buf))
	error("No key obtained\n");
    return buf;
}
    
