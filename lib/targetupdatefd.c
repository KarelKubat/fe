#include "../fe.h"

void target_update_fd(char const *f, int fd) {
    struct stat statbuf;
    int i;
    char *thisrealp, *targetrealp;

    /* Don't store bad fd's or non-existing files. The file must exist
     * by now, we're called from open(), close() etc.
     */
    if (fd < 3 || fstat(fd, &statbuf) || !isfile(&statbuf))
	return;

    /* Examine what we have */
    thisrealp = xrealpath(f);
    for (i = 0; i < fectx()->ntargets; i++) {
	targetrealp = xrealpath(fectx()->targets[i].name);
	if (!strcmp(thisrealp, targetrealp)) {
	    fectx()->targets[i].fd = fd;
	    msg(fectx(), "Candidate %s, fd %d: exists and is a target\n",
		f, fd);
	    free(targetrealp);
	    break;
	}
	free(targetrealp);
    }
    free(thisrealp);
}

    
