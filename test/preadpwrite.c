#include "../fe.h"

char fname[] = "/tmp/fe-preadpwrite.txt";
char buf1[]   =
    "01234567890123456789012345678901234567890123456789"
    "01234567890123456789012345678901234567890123456789";
char buf2[256];

int main() {
    int fd, i;
    static FeCtx ctx;
    off_t off;

    randinit(&ctx, "user secret");
    target_add(&ctx, fname);
    ctx.msg_verbosity = 1;
    setenv("FE_CTX", fectx_serialize(&ctx), 1);

    if ( (fd = open(fname, O_CREAT | O_WRONLY, 0644)) < 0 )
	error("Cannot write %s: %s\n", fname, strerror(errno));
    for (i = 0, off = 0; i < 1000; i++, off += strlen(buf1))
	pwrite(fd, buf1, strlen(buf1), off);
    close(fd);

    if ( (fd = open(fname, O_RDONLY)) < 0 )
	error("Cannot read back %s: %s\n", fname, strerror(errno));
    for (i = 0; i < 1000; i++) {
	off = lseek(fd, 0, SEEK_CUR);
	read(fd, buf2, 100);
	if (strcmp(buf2, buf1))
	    error("Mismatch at offset %ld: "
		  "read back %s, expected %s\n", (long)off, buf2, buf1);
    }
    close(fd);

    return 0;
}
	
    
	
