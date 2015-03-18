#include "../fe.h"

void fe_target_update_fd(char const *f, int fd) {
    struct stat statbuf;
    int i;
    char *thisrealp, *targetrealp;

    /* Don't store bad fd's or non-existing files. The file must exist
     * by now, we're called from open(), close() etc.
     */
    if (fd < 3 || fstat(fd, &statbuf) || !fe_isfile(&statbuf))
	return;

    /* Examine what we have */
    thisrealp = fe_xrealpath(f);
    for (i = 0; i < fectx()->ntargets; i++) {
	targetrealp = fe_xrealpath(fectx()->targets[i].name);
	if (!strcmp(thisrealp, targetrealp)) {
	    fectx()->targets[i].fd = fd;
	    fe_msg(fectx(), "Candidate %s, fd %d: exists and is a target\n",
		f, fd);
	    free(targetrealp);
	    break;
	}
	free(targetrealp);
    }
    free(thisrealp);
}

    
