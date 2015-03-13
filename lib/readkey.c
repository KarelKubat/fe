#include "../fe.h"

char *readkey(void) {
    static char buf[256];
    char *cp;

    read(0, buf, sizeof(buf) - 1);
    if ( (cp = strchr(buf, '\n')) )
	*cp = 0;
    if (! strlen(buf))
	error("No key obtained\n");
    return buf;
}
    
