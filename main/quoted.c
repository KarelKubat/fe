#include "../fe.h"

static char *surround(char const *s, char ch) {
    char *ret = 0;
    char sur[2];

    sur[0] = ch;
    sur[1] = 0;

    ret = fe_xstrdup(sur);
    ret = fe_xstrcat(ret, s);
    ret = fe_xstrcat(ret, sur);

    return ret;
}

char *quoted(char const *s) {
    if (! strchr(s, ' ') )
	return fe_xstrdup(s);
    if (! strchr(s, '\'') )
	return surround(s, '\'');
    if (! strchr(s, '"') )
	return surround(s, '"');
    fe_error("Cannot shell-quote [%s] (not yet implemented)\n", s);
    return 0;
}
