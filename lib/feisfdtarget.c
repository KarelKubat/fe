#include "../fe.h"

int fe_is_fd_target(int fd) {
    int i;

#if 0
    if (fectx()->ntargets > 0) {
	fe_msg(fectx(), "Target overview:\n");
	for (i = 0; i < fectx()->ntargets; i++)
	    fe_msg(fectx(), "Target fd %d of %s\n",
		   fectx()->targets[i].fd, fectx()->targets[i].name);
    }
#endif

    /* Don't consider invalid fd's */
    if (fd < 0)
	return 0;

    /* Match in our target list */
    for (i = 0; i < fectx()->ntargets; i++)
	if (fectx()->targets[i].fd == fd) {
	    fe_msg(fectx(), "Fd %d: is a transcryption target for %s\n",
		   fd, fectx()->targets[i].name);
	    return 1;
	}

#if 0
    fe_msg(fectx(), "Fd %d: not in target table\n", fd);
#endif
    return 0;
}
