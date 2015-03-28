#include "../fe.h"

FeCtx *fectx() {
    static FeCtx *ctx;
    char *env, *cp;
    int shmid;
    void *mem;

    /* Already decoded? Return it right away */
    if (ctx)
	return ctx;

    /* If all are digits then we have a shm id. Else the ctx is in
     * the environment variable itself. */
    if (! (env = getenv("FE_CTX")) )
	fe_error("Environment not set up correctly\n");
    for (cp = env; *cp; cp++)
	if (! isdigit(*cp)) {
	    ctx = fectx_deserialize(env);
	    return ctx;
	}

    /* Only found digits, must be a shm id */
    sscanf(env, "%d", &shmid);
    mem = fe_shmat(shmid);
    ctx = fectx_deserialize( (char*) mem );
    return ctx;
}
