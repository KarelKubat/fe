#include "../fe.h"

char fe_randbyte(FeCtx *ctx, uint32_t x, BitSequence *hashval) {
    static int key_initialized = 0;
    static BitSequence rehashbuffer[HASH_BYTE_SIZE];
    HashReturn ret;
    static uint32_t last_x, required_x;
    char *pr;

    /* Initialize the hashfield using a key if required. */
    if (!key_initialized) {
        key_initialized++;
        if ( (ret = fe_Hash(HASH_BIT_SIZE, (BitSequence const *)ctx->seed,
                            strlen(ctx->seed) * 8, rehashbuffer)) != SUCCESS )
            fe_error("Hash algorithm indicates error during prehash: %d\n",
                     (int)ret);
      if (ctx->msg_verbosity) {
          pr = fe_printable_bytes(rehashbuffer, HASH_BYTE_SIZE);
          fe_msg(ctx, "Initial rehashbuffer: %s\n", pr);
          free(pr);
      }
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

        if (ctx->msg_verbosity) {
            pr = fe_printable_bytes(rehashbuffer, HASH_BYTE_SIZE);
            fe_msg(ctx, "Rehash buffer for offset %d: %s\n", x, pr);
            free(pr);
        }

        /* Convert rehashbuffer into hashbits for usage */
        if ( (ret = fe_Hash(HASH_BIT_SIZE, (BitSequence const *) rehashbuffer,
                            HASH_BIT_SIZE, hashval)) != SUCCESS )
            fe_error("Hash algorithm indicates error %d\n", (int)ret);

        if (ctx->msg_verbosity) {
            pr = fe_printable_bytes(hashval, HASH_BYTE_SIZE);
            fe_msg(ctx, "Hashval for offset %d: %s\n", x, pr);
            free(pr);
        }

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
