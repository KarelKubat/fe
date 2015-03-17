#include "../fe.h"
#include "magic.h"


static char *getmagic() {
    static int magic_converted = 0;
    int i, counter = 131;

    if (! magic_converted) {
	for (i = 0; i < sizeof(magic) - 1; i++)
	    magic[i] -= counter++;
	magic_converted++;
    }

    return magic;
}

char *seed_serialize(char const *buf) {
    char *ret = 0, *tmp, ch;
    int i;
    BitSequence hashval[HASH_BYTE_SIZE];

    /* Make sure a next hashblock is obtained */
    hashval[0] = 0;

    /* Encrypt seed using magic */
    for (i = 0; i < strlen(buf); i++) {
	ch = buf[i] ^ randbyte_keyed(getmagic(), (uint32_t)i, hashval);
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
    unsigned int ui;
    BitSequence hashval[HASH_BYTE_SIZE];
    
    /* Make sure a next hashblock is obtained */
    hashval[0] = 0;

    /* Decrypt seed using magic */
    for (i = 0; i < strlen(buf); i += 2) {
	sscanf(buf + i, "%2x", &ui);
	b2[0] = (char)ui;
	b2[0] ^= randbyte_keyed(getmagic(), (uint32_t) i / 2, hashval);
	ret = xstrcat(ret, b2);
    }
    
    return ret;    
}
