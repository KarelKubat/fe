#include "../fe.h"

#define BYTES_TO_SHOW 100

void fe_cryptbuf(unsigned char *buf, size_t bufsz, size_t offset,
                 BitSequence *hashval) {
    size_t i;
    char *pr = 0;
    unsigned char *bufp;

    fe_msg(fectx(), "Transcrypting buffer of %lu bytes (offset %lu)\n",
	   (unsigned long)bufsz, (unsigned long)offset);

    if (! fectx()->debug)
	for (i = 0, bufp = buf;  i < bufsz;  i++, bufp++, offset++)
          *bufp ^= fe_randbyte(fectx(), offset, hashval);

    if (fectx()->msg_verbosity) {
        pr = fe_printable_bytes(buf,
                                bufsz < BYTES_TO_SHOW ? bufsz : BYTES_TO_SHOW);
	fe_msg(fectx(), "Transcryption result (cut off at %d bytes): %s\n",
	       BYTES_TO_SHOW, pr);
	free(pr);
    }
}
