#include "../fe.h"

char fname[] = "/tmp/fe-readwrite.txt";
char buf1[]   =
    "01234567890123456789012345678901234567890123456789"
    "01234567890123456789012345678901234567890123456789";
char buf2[256];

int main() {
    int fd, i;
    FeCtx *ctx;
    off_t off;

    ctx = fe_setup("user secret", 1, dst_stderr, 1, 1, 0);
    fe_target_add(ctx, fname);

    if ( (fd = open(fname, O_CREAT | O_WRONLY, 0644)) < 0 )
	fe_error("Cannot write %s: %s\n", fname, strerror(errno));
    for (i = 0; i < 1000; i++)
	write(fd, buf1, strlen(buf1));
    close(fd);

    if ( (fd = open(fname, O_RDONLY)) < 0 )
	fe_error("Cannot read back %s: %s\n", fname, strerror(errno));
    for (i = 0; i < 1000; i++) {
	off = lseek(fd, 0, SEEK_CUR);
	read(fd, buf2, 100);
	if (strcmp(buf2, buf1))
	    fe_error("Mismatch at offset %ld: "
		  "read back %s, expected %s\n", (long)off, buf2, buf1);
    }
    close(fd);

    return 0;
}
