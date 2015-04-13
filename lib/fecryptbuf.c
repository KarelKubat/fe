#include "../fe.h"

#define BYTES_TO_SHOW 100

void fe_cryptbuf(char *buf, size_t bufsz, size_t offset, BitSequence *hashval) {
    size_t i;
    char *bufp, *pr = 0, conv[10];

    fe_msg(fectx(), "Transcrypting buffer of %lu bytes (offset %lu)\n",
	   (unsigned long)bufsz, (unsigned long)offset);

    if (! fectx()->debug)
	for (i = 0, bufp = buf;  i < bufsz;  i++, bufp++, offset++)
	    *bufp ^= fe_randbyte(fectx(), offset, hashval);
    
    if (fectx()->msg_verbosity) {
	for (i = 0, bufp = buf; i < BYTES_TO_SHOW && i < bufsz; i++, bufp++) {
	    switch (*buf) {
	    case '\a':
		pr = fe_xstrcat(pr, "\\a");
		break;
	    case '\b':
		pr = fe_xstrcat(pr, "\\b");
		break;
	    case '\n':
		pr = fe_xstrcat(pr, "\\n");
		break;
	    case '\r':
		pr = fe_xstrcat(pr, "\\r");
		break;
	    case '\t':
		pr = fe_xstrcat(pr, "\\t");
		break;
	    case '\v':
		pr = fe_xstrcat(pr, "\\v");
		break;
	    default:
		if (isprint(*bufp)) {
		    conv[0] = *bufp;
		    conv[1] = 0;
		} else
		    sprintf(conv, "\\%3.3o", *bufp & 0xff);
		pr = fe_xstrcat(pr, conv);
		break;
	    }
	}
	fe_msg(fectx(), "Transcryption result (max %d bytes): %s\n",
	       BYTES_TO_SHOW, pr);
	free(pr);
    }
}
