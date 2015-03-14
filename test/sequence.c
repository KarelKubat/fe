#include "../fe.h"

FeCtx ctx;
int main() {
    int i, s[10000], r;

    ctx.seed = "secret";
    
    for (i = 0; i <= 10000; i++)
	s[i] = randbyte(&ctx, i);
    printf("Sequence generated, first 100 values:\n");
    for (i = 0; i < 100; i++)
	printf("%d ", s[i]);
    printf("\n");
    
    for (i = 0; i < 100; i++)
	if (s[i + 1] == s[i])
	    error("Duplicate value at index %d\n", i);

    for (i = 10000; i >= 0; i--) {
	r = randbyte(&ctx, i);
	if (s[i] != r)
	    error("Sequence test failed at index %d\n", i);
    }
    
    return 0;
}
