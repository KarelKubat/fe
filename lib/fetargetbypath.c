#include "../fe.h"

void fe_target_by_path(char const *f, int newfd) {
    struct stat statbuf;
    int i;
    char *thisrealp, *targetrealp;

    /* Never mind non-regular files */
    if (fstat(newfd, &statbuf) || !fe_isfile(&statbuf))
	return;

    /* Examine what we have */
    thisrealp = fe_xrealpath(f);
    for (i = 0; i < fectx()->ntargets; i++) {
	targetrealp = fe_xrealpath(fectx()->targets[i].name);
	if (!strcmp(thisrealp, targetrealp)) {
	    fectx()->targets[i].fd = newfd;
	    fe_msg(fectx(), "File %s, fd %d: open as target\n", 
		f, newfd);
	    free(targetrealp);
	    break;
	}
	free(targetrealp);
    }
    free(thisrealp);
}

    
