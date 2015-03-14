#include "../fe.h"
#include "SHA3api_ref.h"

char randbyte(FeCtx *ctx, uint32_t x) {
    char *data;
    BitSequence hashval[64];
    HashReturn ret;

    asprintf(&data, "%u", x);
    data = xstrcat(data, ctx->seed);
    
    if ( (ret = Hash(512, (BitSequence const *) data, strlen(data),
		     hashval)) != SUCCESS )
	error("Skein512 indicates error %d\n", (int)ret);

    free(data);
    return hashval[x % 64];
}

