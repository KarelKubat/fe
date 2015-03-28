#include "../fe.h"

static char *ctxenv;
static int shmid = -1;

void fectx_unset() {
    struct shmid_ds sds;
    
    free(ctxenv);
    ctxenv = 0;

    if (shmid != -1) {
	/* Note: no error checks here, on purpose, we don't want to trigger
	   exit() because it would bring us right back here */
	shmctl(shmid, IPC_RMID, &sds);
	shmid = -1;
    }
}

void fectx_set(FeCtx const *ctx) {
    char *mem;
    
    fectx_unset();
    
    ctxenv = fectx_serialize(ctx);
    fe_msg(ctx, "Serialized context: %s\n", ctxenv);

    /* If requested, poke into the environment */
    if (ctx->use_env) {
	setenv("FE_CTX", ctxenv, 1);
	return;
    }

    /* Else, poke into shmem */
    if ( (shmid = shmget(IPC_PRIVATE, strlen(ctxenv + 1),
			 IPC_CREAT | 0644)) < 0 )
	fe_error("Failed to get shared memory: %s\n", strerror(errno));
    fe_msg(ctx, "Shared memory block at id %d\n", shmid);
    mem = fe_shmat(shmid);
    strcpy(mem, ctxenv);

    /* Set value of FE_CTX to the id */
    free(ctxenv);
    fe_xasprintf(&ctxenv, "%d", shmid);
    setenv("FE_CTX", ctxenv, 1);
    
}

