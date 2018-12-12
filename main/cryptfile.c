#include "../fe.h"

/* Read/write size */
#define BLOCKSIZE 102400

/* Max dots on progress bar */
#define MAXDOTS 50

/* Max length of progress bar. We make no attempt to determine the actual
 * window size. */
#define LINELEN 78

void cryptfile(char const *f) {
    int fd, i, j;
    unsigned char buf[BLOCKSIZE];
    size_t offset;
    BitSequence hashval[HASH_BYTE_SIZE];
    off_t totread = 0;
    struct stat statbuf;
    time_t start_time = time(0), complete_time = 0,
	   elapsed_time = 1, last_elapsed = 0;
    char *secdisp;

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
		printf("\r[");
		for (i = 0;
                     i < (MAXDOTS * (double)totread / statbuf.st_size);
                     ++i)
		    putchar('.');
		for (j = i; j < MAXDOTS; j++)
		    putchar(' ');
                fe_xasprintf(&secdisp, " %lus (%lus left)]",
                             (long unsigned) elapsed_time,
                             (long unsigned) complete_time - elapsed_time);
                printf("%s", secdisp);
                /* Add 2 for leading and trailing [] */
                for (i = 2 + MAXDOTS + (int)strlen(secdisp); i < LINELEN; ++i)
                  putchar(' ');
                free(secdisp);
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
    printf("\n");

    if (close(fd) < 0)
	fe_error("Close error on file %s: %s\n", f, strerror(errno));

    fe_msg(fectx(), "File %s transcrypted.\n", f);
}
