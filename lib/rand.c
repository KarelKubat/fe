#include "../fe.h"

/* Background: see http://en.wikipedia.org/wiki/Xorshift */

char randbyte(FeCtx *ctx, uint32_t x) {
    x ^= ctx->seed;
    
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

/*--- ElfHash ---------------------------------------------------
 *  The published hash algorithm used in the UNIX ELF format
 *  for object files. Accepts a pointer to a string to be hashed
 *  and returns an unsigned long.
 * See http://www.drdobbs.com/database/hashing-rehashed/184409859
 *-------------------------------------------------------------*/
static unsigned long ElfHash(const unsigned char *name) {
    unsigned long   h = 0, g;
    while (*name) {
        h = ( h << 4 ) + *name++;
        if ( (g = h & 0xF0000000) )
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}

void randinit(FeCtx *ctx, char const *buf) {
    ctx->seed = (uint32_t) ElfHash((const unsigned char *)buf);
}
