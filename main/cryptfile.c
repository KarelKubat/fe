#include "../fe.h"

void cryptfile(char const *f) {
    int fd;
    char buf[102400];
    size_t offset;
    BitSequence hashval[HASH_BYTE_SIZE];

    /* Open or croak */
    if ( (fd = open(f, O_RDWR)) < 0 )
	fe_error("Cannot open file %s: %s\n", f, strerror(errno));

    /* Make sure transcryptor rehashes */
    *hashval = 0;

    /* Fetch blocks, transcrypt, write back */
    while (1) {
	ssize_t nread, nwritten;

	/* Get starting offset */
	offset = lseek(fd, 0, SEEK_CUR);

	/* Get next block */
	nread = read(fd, buf, sizeof(buf));
	if (! nread)
	    break;
	if (nread < 0)
	    fe_error("Read error on file %s: %s\n", f, strerror(errno));

	/* Encrypt */
	fe_cryptbuf(buf, nread, offset, hashval);

	/* Rewind to previous offset */
	if (lseek(fd, offset, SEEK_SET) < 0)
	    fe_error("Seek error on file %s: %s\n", f, strerror(errno));

	/* Write buffer into file */
	nwritten = write(fd, buf, nread);
	if (nwritten < 0)
	    fe_error("Write error on file %s: %s\n", f, strerror(errno));
	if (nwritten != nread)
	    fe_error("Write error on file %s: "
		  "only %d bytes written instead of %d\n",
		  f, (int)nwritten, (int)nread);
    }
    
    if (close(fd) < 0)
	fe_error("Close error on file %s: %s\n", f, strerror(errno));

    fe_msg(fectx(), "File %s transcrypted.\n", f);
}
