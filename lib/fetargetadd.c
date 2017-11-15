#include "../fe.h"

/* Returns an emacs tmp buffer name for a given file,
 * /where/ever/file.txt -> /where/ever/.#file.txt
 */
static char *emacsbuf(char *t) {
    char *ret = 0;
    char *dn = fe_xdirname(t);

    if (strcmp(dn, ".")) {
	dn = fe_xstrcat(dn, "/");
	ret = dn;
    } else
	free(dn);

    ret = fe_xstrcat(ret, ".#");
    ret = fe_xstrcat(ret, fe_xbasename(t));
    return ret;
}

/* Returns a vi tmp buffer for a given file,
 * /where/ever/file.txt -> /where/ever/.file.txt.swp
 */
static char *vibuf(char *t) {
    char *ret = 0;
    char *dn = fe_xdirname(t);

    if (strcmp(dn, ".")) {
	dn = fe_xstrcat(ret, "/");
	ret = dn;
    } else
	free(dn);

    ret = fe_xstrcat(ret, ".");
    ret = fe_xstrcat(ret, fe_xbasename(t));
    ret = fe_xstrcat(ret, ".swp");
    return ret;
}

void fe_target_add(FeCtx *ctx, char const *t) {
    struct stat statbuf;
    char *fname;
    int statres;

    statres = stat(t, &statbuf);

    /* If the stat works, then only consider the file if it's a true file.
     * If stat doesn't work, then consider it - it may appear later.
     * In any case, use the real path to the file.
     */
    if ( (!statres && fe_isfile(&statbuf)) ||
	 (statres) ) {
	ctx->targets = fe_xrealloc(ctx->targets,
                                   (ctx->ntargets + 3) * sizeof(Target));

	if (! (fname = realpath(t, 0)) )
          fe_error("Failed to resolve path for %s: %s\n", t, strerror(errno));

	ctx->targets[ctx->ntargets].name = fname;
	ctx->targets[ctx->ntargets].fd = -1;

	ctx->targets[ctx->ntargets + 1].name = emacsbuf(fname);
	ctx->targets[ctx->ntargets + 1].fd = -1;

	ctx->targets[ctx->ntargets + 2].name = vibuf(fname);
	ctx->targets[ctx->ntargets + 2].fd = -1;

	ctx->ntargets += 3;
    }

    /* Poke new or pre-existing context into environment or shared mem */
    fectx_set(ctx);
}
