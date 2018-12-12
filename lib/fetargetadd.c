#include "../fe.h"

/* Returns an emacs tmp buffer name for a given file,
 * /where/ever/file.txt -> /where/ever/.#file.txt
 */
static char *emacsbuf(char *t) {
    char *dirpart  = fe_xdirname(t);
    char *basepart = fe_xbasename(t);

    dirpart = fe_xstrcat(dirpart, "/.#");
    dirpart = fe_xstrcat(dirpart, basepart);

    return dirpart;
}

/* Returns a vi tmp buffer for a given file,
 * /where/ever/file.txt -> /where/ever/.file.txt.swp
 */
static char *vibuf(char *t) {
    char *dirpart  = fe_xdirname(t);
    char *basepart = fe_xbasename(t);

    dirpart = fe_xstrcat(dirpart, "/.");
    dirpart = fe_xstrcat(dirpart, basepart);
    dirpart = fe_xstrcat(dirpart, ".swp");

    return dirpart;
}

void fe_target_add(FeCtx *ctx, char const *t) {
    struct stat statbuf;
    char *real_fname;
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

        real_fname = fe_xrealpath(t);

	ctx->targets[ctx->ntargets].name = real_fname;
	ctx->targets[ctx->ntargets].fd = -1;

	ctx->targets[ctx->ntargets + 1].name = emacsbuf(real_fname);
	ctx->targets[ctx->ntargets + 1].fd = -1;

	ctx->targets[ctx->ntargets + 2].name = vibuf(real_fname);
	ctx->targets[ctx->ntargets + 2].fd = -1;

	ctx->ntargets += 3;
    }

    /* Poke new or pre-existing context into environment or shared mem */
    fectx_set(ctx);
}
