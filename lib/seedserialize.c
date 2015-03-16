#include "../fe.h"

#ifndef MAGIC
#error "You need to define a MAGIC string for the build. RTFM please."
#endif

char *seed_serialize(char const *buf) {
    char *ret = 0, *tmp, ch;
    int i;

    for (i = 0; i < strlen(buf); i++) {
	ch = buf[i] ^ randbyte_keyed(MAGIC, (uint32_t)i);
	xasprintf(&tmp, "%2.2x", (ch & 0xff));
	ret = xstrcat(ret, tmp);
	free(tmp);
    }
    return ret;
}

char *seed_deserialize(char const *buf) {
    char *ret = 0;
    int i;
    char b2[2] = {0, 0};

    for (i = 0; i < strlen(buf); i += 2) {
	b2[0] = (buf[i] << 8 | buf[i + 1]) ^
	    randbyte_keyed(MAGIC, (uint32_t) i / 2);
	ret = xstrcat(ret, b2);
    }
    return ret;
    
}
