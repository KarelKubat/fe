#include "../fe.h"

char fe_randbyte(FeCtx *ctx, uint32_t x, BitSequence *hashval) {
    static int key_initialized;
    static BitSequence rehashbuffer[HASH_BYTE_SIZE];
    HashReturn ret;
    static uint32_t last_x, required_x;

    /* Initialize the hashfield using a key if required. */
    if (!key_initialized) {
        key_initialized++;
        if ( (ret = fe_Hash(HASH_BIT_SIZE, (BitSequence const *)ctx->seed,
                            strlen(ctx->seed), rehashbuffer)) != SUCCESS )
            fe_error("Hash algorithm indicates error during prehash: %d\n",
                     (int)ret);
    }

    /* Need to recalculate the hash if:
       - there is no previous
       - current hash is more than HASH_BYTE_SIZE off
    */
    required_x = (x / HASH_BYTE_SIZE) * HASH_BYTE_SIZE;
    if (! *hashval || last_x != required_x) {

        /* Poke start offset into the rehashbuffer. */
        last_x = required_x;
        memcpy(rehashbuffer, &last_x, sizeof(last_x));

        /* Convert rehashbuffer into hashbits for usage */
        if ( (ret = fe_Hash(HASH_BIT_SIZE, (BitSequence const *) rehashbuffer,
                            HASH_BYTE_SIZE, hashval)) != SUCCESS )
            fe_error("Hash algorithm indicates error %d\n", (int)ret);

        /*
        {
            int i;
            fprintf(stderr, "%s->", template);
            for (i = 0; i < HASH_BYTE_SIZE; i++)
                fprintf(stderr, "%2.2x", hashval[i]);
            fprintf(stderr, "\n");
        }
        */
    }

    /* Next pseudorandom value is in the hashed bits */
    return (hashval[x % HASH_BYTE_SIZE]) & 0xff;
}
