#include "../fe.h"

char writebuf[]  = "Written using fwrite";
char readbuf[255];
FeCtx ctx;

int main() {
    FILE *f;

    randinit(&ctx, "user secret");
    ctx.msg_verbosity = 1;
    target_add(&ctx, "/tmp/bufferedio.txt");
    setenv("FE_CTX", fectx_serialize(&ctx), 1);

    printf("Opening file for writing using fopen...\n");
    if (! (f = fopen("/tmp/bufferedio.txt", "w")) )
	perror("/tmp/bufferedio.txt");

    printf("Writing using fwrite...\n");
    fwrite(writebuf, sizeof(writebuf), 1, f);

    printf("Closing using fclose...\n");
    fclose(f);

    printf("Opening file for reading...\n");
    if (! (f = fopen("/tmp/bufferedio.txt", "r")) )
	perror("/tmp/bufferedio.txt");

    fread(readbuf, sizeof(readbuf), 1, f);

    if (strcmp(writebuf, readbuf))
	fe_error("Buffer mismatch, expected: [%s]\n"
		 "                      got: [%s]\n",
		 writebuf, readbuf);

    return 0;
}
