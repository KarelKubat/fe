#include "../fe.h"

int isfile(struct stat const *st) {
    mode_t m = st->st_mode;

    if (m & S_IFIFO || m & S_IFCHR || m & S_IFDIR || m & S_IFBLK)
	return 0;
    return m & S_IFREG;
}
