#include "../fe.h"

void fe_target_by_path(char const *f, int newfd) {
    struct stat statbuf;
    int i;
    char *real_fname;
    int org_errno;

    /* Stack errno's */
    org_errno = errno;

    /* Never mind non-regular files */
    if (fstat(newfd, &statbuf) || !fe_isfile(&statbuf)) {
	errno = org_errno;
	return;
    }

    /* Examine what we have */
    real_fname = fe_xrealpath(f);
    for (i = 0; i < fectx()->ntargets; i++) {
	if (!strcmp(real_fname, fectx()->targets[i].name)) {
	    fectx()->targets[i].fd = newfd;
	    fe_msg(fectx(), "File %s, fd %d: open as target\n",
		real_fname, newfd);
	    break;
	}
    }
    free(real_fname);

    errno = org_errno;
}
