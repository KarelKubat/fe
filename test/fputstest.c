#include "../fe.h"

char fname[] = "/tmp/fputs.txt";
char readbuf[1024];

int main() {
    FILE *f;
    FeCtx *ctx;

    ctx = fe_setup("user-secret", 1, dst_stderr, 1, 1, 0);
    fe_target_add(ctx, fname);

    printf("Writing %s using fputs\n", fname);
    if (! (f = fopen(fname, "w")) )
	perror(fname);
    fputs("Hello World", f);
    fclose(f);

    printf("Reading back %s\n", fname);
    if (! (f = fopen(fname, "r")) )
	perror(fname);
    fread(readbuf, 1, sizeof(readbuf), f);
    if (strcmp(readbuf, "Hello World"))
	fe_error("Buffer mismatch, expected: [Hello World]\n"
		 "                      got: [%s]\n",
		 readbuf);

    return 0;
}
	
    
