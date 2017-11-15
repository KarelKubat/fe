#include "../fe.h"

void *fe_shmat(int shmid) {
    void *mem;

    if ( (mem = shmat(shmid, 0, 0)) == (void*) -1 )
	fe_error("Failed to attach shared memory: %s\n", strerror(errno));

#if 0
    {
	struct shmid_ds sds;

	if (shmctl(shmid, IPC_STAT, &sds) != 0)
	    fe_error("Failed to stat shared memory: %s\n", strerror(errno));
	printf("Shared memory owner   uid : %d\n", (int) sds.shm_perm.uid);
	printf("Shared memory owner   gid : %d\n", (int) sds.shm_perm.gid);
	printf("Shared memory creator uid : %d\n", (int) sds.shm_perm.cuid);
	printf("Shared memory creator gid : %d\n", (int) sds.shm_perm.cgid);
	printf("Shared memory mode        : %d\n", (int) sds.shm_perm.mode);
	printf("Shared memory segment sz  : %d\n", (int) sds.shm_segsz);
    }
#endif

    return mem;
}
