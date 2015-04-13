#include "../fe.h"

#define BLOCKSIZE 102400

void cryptfile(char const *f) {
    int fd, i, j;
    char buf[BLOCKSIZE];
    size_t offset;
    BitSequence hashval[HASH_BYTE_SIZE];
    off_t totread = 0;
    struct stat statbuf;

    /* Open or croak */
    if (stat(f, &statbuf))
	fe_error("Cannot stat file %s: %s\n", f, strerror(errno));
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
	totread += nread;

	/* Update progress meter */
#if 0	
	{
	    int ndots = (int) (50 * totread / statbuf.st_size);
	    printf("read %ld of total %ld, dots=%d\n",
		   (long)totread, (long)statbuf.st_size, ndots);
	}
#endif
	if (isatty(1)) {
	    printf("\r[");
	    for (i = 0; i < (75 * totread / statbuf.st_size); i++)
		putchar('.');
	    for (j = i; j < 75; j++)
		putchar(' ');
	    putchar(']');
	    fflush(stdout);
	}

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
    putchar('\n');
    
    if (close(fd) < 0)
	fe_error("Close error on file %s: %s\n", f, strerror(errno));

    fe_msg(fectx(), "File %s transcrypted.\n", f);
}
