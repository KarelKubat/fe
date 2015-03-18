#include "../fe.h"

void fe_cryptbuf(char *buf, size_t bufsz, size_t offset, BitSequence *hashval) {
    size_t i;

    fe_msg(fectx(), "Transcrypting buffer of %lu bytes (offset %lu)\n",
	   (unsigned long)bufsz, (unsigned long)offset);
    
    for (i = 0; i < bufsz; i++) {
	*buf ^= fe_randbyte(fectx(), offset, hashval);
	buf++;
	offset++;
    }
}
