#include "../fe.h"

void target_update_fd(char const *f, int fd) {
    struct stat statbuf;
    int i;

    /* Don't store bad fd's or non-existing files*/
    if (fd < 0) {
	msg(fectx(), "Candidate fd %d: invalid fd, not a target\n", fd);
	return;
    }
    if (fstat(fd, &statbuf)) {
	msg(fectx(), "Candidate file %s: cannot stat, not a target\n", f);
	return;
    }

    /* Examine what we have */
    for (i = 0; i < fectx()->ntargets; i++) {
	if (fectx()->targets[i].st_dev == statbuf.st_dev &&
	    fectx()->targets[i].st_ino == statbuf.st_ino) {
	    fectx()->targets[i].fd = fd;
	    msg(fectx(), "Candidate %s, fd %d: exists and is a target\n",
		f, fd);
	    break;
	}
    }
}

    
