#include "../fe.h"

int fe_xvasprintf(char **bufp, char const *fmt, va_list args) {
    int ret;

    if ( (ret = vasprintf(bufp, fmt, args)) < 0 )
	fe_error("Out of memory\n");
    return ret;
}
