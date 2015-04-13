#include "../fe.h"

/* Local functions for the main program, undeclared in fe.h */
extern void cryptfile(char const *f);
extern char *quoted(char const *s);
extern void usage(void);

int main(int argc, char **argv) {
    int opt, target_set = 0, i, ret = 0;
    char *file_to_crypt = 0, buffer[1024], *sysbuf, *key = 0, *key_again, *cp;
    static FeCtx ctx;

    /* Catchall usage info */
    if (argc == 1)
	usage();

    /* Parse command line */
    while ( (opt = getopt(argc, argv, "h?k:f:t:vsiVed")) > -1 )
	switch (opt) {
	case 'h':
	case '?':
	    usage();
	case 'k':
	    key = optarg;
	    break;
	case 'f':
	    file_to_crypt = optarg;
	    break;
	case 't':
	    fe_target_add(&ctx, optarg);
	    target_set++;
	    break;
	case 'v':
	    ctx.msg_verbosity = 1;
	    break;
	case 's':
	    ctx.msg_dst = dst_stderr;
	    break;
	case 'i':
	    ctx.ignore_noncaught = 1;
	    break;
	case 'V':
	    printf(VER "\n");
	    return 0;
	case 'e':
	    ctx.use_env = 1;
	    break;
	case 'd':
	    ctx.debug = 1;
	    ctx.msg_verbosity = 1;
	    break;
	}

    /* When not in debug mode: get the crypto key unless given already.
     * Debug key is set to an arbitrary string, it won't be used anywhere,
     * see fe_cryptbuf() */
    if (ctx.debug)
	key = "debug key";
    if (! key)
	key = getenv("FE_KEY");
    if (! key) {
	if (isatty(0)) {
	    key = fe_xstrdup(getpass("fe key : "));
	    key_again      = getpass("again  : ");
	    if (strcmp(key, key_again))
		fe_error("Key entry mismatch\n");
	} else {
	    fgets(buffer, sizeof(buffer) - 1, stdin);
	    if ( (cp = strchr(buffer, '\n')) )
		*cp = 0;
	    key = fe_xstrdup(buffer);
	}
	    
    }
    if (! *key)
	fe_error("Empty key\n");
	    
    ctx.seed = fe_xstrdup(key);

    /* Find targets, if not done so yet via flags -t */
    if (! target_set)
	for (i = optind + 1; i < argc; i++)
	    fe_target_add(&ctx, argv[i]);
    if (ctx.msg_verbosity)
	for (i = 0; i < ctx.ntargets; i++)
	    fe_msg(&ctx, "Transcryption target: %s\n", ctx.targets[i].name);

    /* Set FE environment */
    fectx_set(&ctx);
    atexit(fectx_unset);

    /* Ignore signals that we can ignore, we don't want to stop while
     * transcrypting.
     */
    signal(SIGHUP,  SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    if (file_to_crypt) {
	if (optind != argc)
	    fe_error("No command and arguments accepted with flag -f\n");
	cryptfile(file_to_crypt);
    } else if (optind == argc) {
	fe_error("No action taken, try 'fe -h' for an overview\n");
    } else {
	/* Going to launch the command with the shared object underneath */
	
#if USYS == UN_DARWIN
	/* For MacOSX: */
	setenv("DYLD_INSERT_LIBRARIES", LIBDIR "/" LIB, 1);
	setenv("DYLD_FORCE_FLAT_NAMESPACE", "1", 1);
#elif USYS == UN_LINUX
	/* For Linux: */
	setenv("LD_PRELOAD", LIBDIR "/" LIB, 1);
#else
#error "uname-based system unknown, check the Makefile and make sure \
        that USYS is set"
#endif

	/* Run the intended command */
	sysbuf = 0;
	for (i = optind; i < argc; i++) {
	    char *q = quoted(argv[i]);
	    sysbuf = fe_xstrcat(sysbuf, q);
	    if (i < argc - 1)
		sysbuf = fe_xstrcat(sysbuf, " ");
	}
	fe_msg(&ctx, "About to run command: %s\n", sysbuf);
	if ( (ret = system(sysbuf)) == -1 )
	    fe_error("Failed to run command: cannot fork\n");
	else if (ret == 127)
	    fe_error("Failed to run command: shell failure\n");
    }

    return ret;
}
	       
