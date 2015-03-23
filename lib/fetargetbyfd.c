#include "../fe.h"

void fe_target_by_fd(int oldfd, int newfd) {
    int i, oldfd_index = -1, newfd_index = -1;
    struct stat statbuf;

    /* Never mind non-regular files */
    if (fstat(newfd, &statbuf) || !fe_isfile(&statbuf))
	return;

    /* Make sure oldfd is in the target list; newfd may be in which case
     * it's overwritten */
    for (i = 0; i < fectx()->ntargets; i++) {
	int thisfd = fectx()->targets[i].fd;
	if (thisfd == oldfd)
	    oldfd_index = i;
	if (thisfd == newfd)
	    newfd_index = i;
    }	
    if (oldfd_index == -1)
	return;

    /* Extend the targets structure with a new copy entry, or put newfd
     * at newfd_index */
    if (newfd_index == -1) {
	fectx()->targets =
	    fe_xrealloc(fectx()->targets,
			(fectx()->ntargets + 1) * sizeof(Target));
	newfd_index = fectx()->ntargets;
	fectx()->ntargets++;
    }
    fectx()->targets[newfd_index].fd = newfd;
    fectx()->targets[newfd_index].name =
	fe_xstrdup(fectx()->targets[oldfd_index].name);
    fe_msg(fectx(), "File %s, fd %d: duplicated as target\n",
	   fectx()->targets[oldfd_index].name, newfd);
}
