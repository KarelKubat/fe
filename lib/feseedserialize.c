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

char *fe_seed_serialize(char const *buf) {
    char *ret = 0, *tmp;
    int i, j;
    BitSequence magichash[HASH_BYTE_SIZE];
    HashReturn r;

    /* No seed? No output */
    if (!buf)
	return fe_xstrdup(buf);

    /* Get a hash of our own magic */
    if ( (r = fe_Hash(HASH_BIT_SIZE, (BitSequence const *)getmagic(),
		      strlen(magic), magichash)) )
	fe_error("Hash algorithm indicates error %d\n", (int)r);
    
    /* Encrypt seed using magic. */
    for (i = 0, j = 0; i < HASH_BYTE_SIZE && j < strlen(buf); i++, j++) 
	magichash[i] ^= buf[j];
    /* Build up as one string */
    for (i = 0; i < HASH_BYTE_SIZE; i++) {
	fe_xasprintf(&tmp, "%2.2x", (magichash[i] & 0xff));
	ret = fe_xstrcat(ret, tmp);
	free(tmp);
    }

#if 0
    fprintf(stderr, "Seed serialized to: [%s]\n", ret);
#endif	
    
    return ret;
}

char *fe_seed_deserialize(char const *buf) {
    int i;
    unsigned int ui;
    BitSequence magichash[HASH_BYTE_SIZE + 1];
    HashReturn r;

#if 0    
    fprintf(stderr, "Deserializing seed: [%s]\n", buf);
#endif
    
    /* Terminate hash stuff */
    memset(magichash, 0, sizeof(magichash));
    
    /* Get a hash of our own magic */
    if ( (r = fe_Hash(HASH_BIT_SIZE, (BitSequence const *)getmagic(),
		      strlen(magic), magichash)) )
	fe_error("Hash algorithm indicates error %d\n", (int)r);
    
    /* Decrypt seed using magic */
    for (i = 0; i < strlen(buf); i += 2) {
	sscanf(buf + i, "%2x", &ui);
	magichash[i / 2] ^= (char)ui;
    }
    
    return fe_xstrdup( (char*) magichash );
}
