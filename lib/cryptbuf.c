#include "../fe.h"

void cryptbuf(char *buf, size_t bufsz, size_t offset) {
    size_t i;

    msg(fectx(), "Transcrypting buffer of %lu bytes (offset %lu)\n",
	bufsz, offset);
    
    for (i = 0; i < bufsz; i++) {
	*buf ^= randbyte(fectx(), offset);
	buf++;
	offset++;
    }
}
