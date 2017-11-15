#include "../fe.h"
#include "timing.h"

#define LOOPS 100000

int main() {
    int i;
    BitSequence hashval[HASH_BYTE_SIZE];
    FeCtx ctx;

    ctx.seed = "key";

    /* Spin up */
    for (i = 0; i < LOOPS * 2; i++)
        fe_randbyte(&ctx, 0, hashval);

    /* Hitting the same hash over again */
    printf("fe_randbyte() with same offset        : ");
    timing_start();
    for (i = 0; i < LOOPS; i++)
        fe_randbyte(&ctx, 0, hashval);
    timing_end();
    printf("%g\n", timing_diff());

    /* Hitting alternating hashes */
    printf("fe_randbyte() with alternating offset : ");
    timing_start();
    for (i = 0; i < LOOPS; i++)
        fe_randbyte(&ctx, i * HASH_BYTE_SIZE, hashval);
    timing_end();
    printf("%g\n", timing_diff());

    return 0;
}
