#include "../fe.h"

FeCtx ctx;
int main() {
    int i, s1[10000], r;

    ctx.seed = 123456;
    
    for (i = 0; i <= 10000; i++)
	s1[i] = randbyte(&ctx, i);

    for (i = 10000; i >= 0; i--) {
	r = randbyte(&ctx, i);
	if (s1[i] != r)
	    error("Sequence test failed at index %d\n", i);
    }
    
    return 0;
}
