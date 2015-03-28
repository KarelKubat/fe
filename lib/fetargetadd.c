#include "../fe.h"

/* Returns an emacs tmp buffer name for a given file,
 * /where/ever/file.txt -> /where/ever/.#file.txt
 */
static char *emacsbuf(char *t) {
    char *ret = 0;
    char *dn = dirname(t);
    if (strcmp(dn, ".")) {
	ret = fe_xstrdup(dn);
	ret = fe_xstrcat(ret, "/");
    }
    ret = fe_xstrcat(ret, ".#");
    ret = fe_xstrcat(ret, fe_xstrdup(basename(t)));
    return ret;
}

/* Returns a vi tmp buffer for a given file,
 * /where/ever/file.txt -> /where/ever/.file.txt.swp
 */
static char *vibuf(char *t) {
    char *ret = 0;
    char *dn = dirname(t);
    if (strcmp(dn, ".")) {
	ret = fe_xstrdup(dn);
	ret = fe_xstrcat(ret, "/");
    }
    ret = fe_xstrcat(ret, ".");
    ret = fe_xstrcat(ret, fe_xstrdup(basename(t)));
    ret = fe_xstrcat(ret, ".swp");
    return ret;
}

void fe_target_add(FeCtx *ctx, char const *t) {
    struct stat statbuf;
    char *fname;
    int statres;

    statres = stat(t, &statbuf);

    /* Only consider true files: stat must work, and must be a true file */
    if ( (!statres && fe_isfile(&statbuf)) ||
	 (statres) ) {
	ctx->targets = fe_xrealloc(ctx->targets,
				(ctx->ntargets + 3) * sizeof(Target));

	fname = fe_xstrdup(t);
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
