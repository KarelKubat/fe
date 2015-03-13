#include "../fe.h"

FeCtx *fectx() {
    static FeCtx *ctx;

    if (!ctx) {
	char *env = getenv("FE_CTX");
	if (!env)
	    error("Internal environment variable FE_CTX missing\n");
	ctx = target_deserialize(env);
    }
    return ctx;
}
