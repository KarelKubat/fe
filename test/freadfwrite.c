#include "../fe.h"

char writebuf[]  = "Written using fwrite";
char readbuf[255];
FeCtx ctx;

int main() {
    FILE *f;
    FeCtx *ctx;

    ctx = fe_setup("user secret", 1, dst_syslog, 1);
    fe_target_add(ctx, "/tmp/bufferedio.txt");

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
