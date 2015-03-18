#include "../fe.h"

FeCtx ctx;
int main() {
    int i, s[10000], r;
    BitSequence hashval[HASH_BYTE_SIZE];

    ctx.seed = "secret";
    ctx.msg_verbosity = 1;
    
    for (i = 0; i <= 10000; i++)
	s[i] = fe_randbyte(&ctx, i, hashval);
    printf("Sequence generated, first 100 values:\n");
    for (i = 0; i < 100; i++)
	printf("%d ", s[i]);
    printf("\n");
    
    for (i = 0; i < 100; i++)
	if (s[i] == s[i + 1] && s[1] == s[i + 2])
	    fe_error("Triplicate value at index %d\n", i);

    for (i = 10000; i >= 0; i--) {
	r = fe_randbyte(&ctx, i, hashval);
	if (s[i] != r)
	    fe_error("Sequence test failed at index %d\n", i);
    }
    
    return 0;
}
