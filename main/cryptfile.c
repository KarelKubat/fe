#include "../fe.h"

#define BLOCKSIZE 102400

void cryptfile(char const *f) {
    int fd, i, j;
    char buf[BLOCKSIZE];
    size_t offset;
    BitSequence hashval[HASH_BYTE_SIZE];
    off_t totread = 0;
    struct stat statbuf;
    time_t start_time = time(0), complete_time = 0,
	   elapsed_time = 1, last_elapsed = 0;

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

	/* Encrypt */
	fe_cryptbuf(buf, nread, offset, hashval);

	/* Update progress meter */
	if (isatty(1)) {
	    elapsed_time = time(0) - start_time;
	    complete_time = ((double)statbuf.st_size / totread) * elapsed_time;

	    /* Update display only if we have a sec or more difference. */
	    if (last_elapsed < elapsed_time) {
		last_elapsed = elapsed_time;
		
/* For debugging the progress bar, set this to 1: */		
#if 0
		{
		    int ndots = (int) (60 * totread / statbuf.st_size);
		    printf("read %ld of total %ld, dots=%d, "
			   "complete_time=%ld, elapsed_time=%ld\n",
			   (long)totread, (long)statbuf.st_size, ndots,
			   (long)complete_time, (long)elapsed_time);
		}
#endif
		
		printf("\r[");
		for (i = 0; i < (60 * (double)totread / statbuf.st_size); i++)
		    putchar('.');
		for (j = i; j < 60; j++)
		    putchar(' ');
		printf("%5lu sec left]",
		       (long unsigned) complete_time - elapsed_time);
		fflush(stdout);
	    }
	}

	/* Rewind to previous offset */
	if (lseek(fd, offset, SEEK_SET) < 0) {
	    if (isatty(1))
		putchar('\n');
	    fe_error("Seek error on file %s: %s\n", f, strerror(errno));
	}

	/* Write buffer into file */
	nwritten = write(fd, buf, nread);
	if (nwritten < 0) {
	    if (isatty(1))
		putchar('\n');
	    fe_error("Write error on file %s: %s\n", f, strerror(errno));
	}
	if (nwritten != nread) {
	    if (isatty(1))
		putchar('\n');
	    fe_error("Write error on file %s: "
		  "only %d bytes written instead of %d\n",
		  f, (int)nwritten, (int)nread);
	}
    }
    printf("\r[");
    for (i = 0; i < 60; i++)
	putchar('.');
    printf("          done]\n");
    
    if (close(fd) < 0)
	fe_error("Close error on file %s: %s\n", f, strerror(errno));

    fe_msg(fectx(), "File %s transcrypted.\n", f);
}
