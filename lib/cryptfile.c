#include "../fe.h"

void cryptfile(char const *f) {
    int fd;
    char buf[51200];
    size_t offset;

    if ( (fd = open(f, O_RDWR)) < 0 )
	error("Cannot open file %s: %s\n", f, strerror(errno));

    while (1) {
	ssize_t nread, nwritten;

	/* Get starting offset */
	offset = lseek(fd, 0, SEEK_CUR);

	/* Get next block */
	nread = read(fd, buf, sizeof(buf));
	if (! nread)
	    break;
	if (nread < 0)
	    error("Read error on file %s: %s\n", f, strerror(errno));

	/* Encrypt */
	cryptbuf(buf, nread, offset);

	/* Rewind to previous offset */
	if (lseek(fd, offset, SEEK_SET) < 0)
	    error("Seek error on file %s: %s\n", f, strerror(errno));

	/* Write buffer into file */
	nwritten = write(fd, buf, nread);
	if (nwritten < 0)
	    error("Write error on file %s: %s\n", f, strerror(errno));
	if (nwritten != nread)
	    error("Write error on file %s: "
		  "only %d bytes written instead of %d\n",
		  f, (int)nwritten, (int)nread);
    }
    
    if (close(fd) < 0)
	error("Close error on file %s: %s\n", f, strerror(errno));

    msg(fectx(), "File %s transcrypted.\n", f);
}
