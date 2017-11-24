#include "../fe.h"

char fname[] = "/tmp/fgetc.txt";
char readbuf[1024];

int main() {
    FILE *f;
    FeCtx *ctx;
    int ch;

    ctx = fe_setup("user-secret",    // secret key
		   1,		     // verbosity
		   dst_stderr,	     // but msg to stderr
		   1,		     // ignore uncaught lib functions
		   1,		     // use environment to transfer, not shmem
		   0);		     // debugging off, real mode on
    fe_target_add(ctx, fname);

    printf("Writing %s using fputs\n", fname);
    if (! (f = fopen(fname, "w")) )
	perror(fname);
    fputs("Hello World", f);
    fclose(f);

    printf("Reading back %s using fgetc\n", fname);
    if (! (f = fopen(fname, "r")) )
	perror(fname);
    while ( (ch = fgetc(f)) != EOF )
	readbuf[strlen(readbuf)] = ch;
    if (strcmp(readbuf, "Hello World"))
	fe_error("Buffer mismatch, expected: [Hello World]\n"
		 "                      got: [%s]\n",
		 readbuf);

    return 0;
}
	
    
