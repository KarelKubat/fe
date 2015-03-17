#include "../fe.h"

char fname[] = "/tmp/fe-readvwritev.txt";

#define BUF1 "This is the first line.\n"
#define BUF2 "This is the second line, just beyond the first one.\n"
#define BUF3 "This is the last line.\n"

struct iovec io[] = {
    { BUF1, sizeof(BUF1)-1 },
    { BUF2, sizeof(BUF2)-1 },
    { BUF3, sizeof(BUF3)-1 }
};

int main() {
    int fd, i;
    static FeCtx ctx;
    struct iovec *dst;

    randinit(&ctx, "user secret");
    ctx.msg_verbosity = 1;
    target_add(&ctx, fname);
    setenv("FE_CTX", fectx_serialize(&ctx), 1);

    if ( (fd = open(fname, O_TRUNC | O_CREAT | O_WRONLY, 0644)) < 0 )
	error("Cannot write %s: %s\n", fname, strerror(errno));
    writev(fd, io, 3);
    close(fd);
    
    if ( (fd = open(fname, O_RDONLY)) < 0 )
	error("Cannot read back %s: %s\n", fname, strerror(errno));
    dst = xmalloc(3 * sizeof(struct iovec));
    for (i = 0; i < 3; i++) {
	dst[i].iov_len  = io[i].iov_len;
	dst[i].iov_base = xmalloc(100);
    }
    readv(fd, dst, 3);
    for (i = 0; i < 3; i++) 
	printf("Read back block %d: %s", i, dst[i].iov_base);
    close(fd);
    
    return 0;
}
	
    
	
