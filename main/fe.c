#include "../fe.h"

/* Local functions for the main program, undeclared in fe.h */
extern void cryptfile(char const *f);
extern void usage(void);

int main(int argc, char **argv) {
    int opt, target_set = 0, i, ret = 0;
    char *file_to_crypt = 0, buffer[1024], *key = 0;
    static FeCtx ctx;

    /* Catchall usage info */
    if (argc == 1)
	usage();

    /* Parse command line */
    while ( (opt = getopt(argc, argv, "h?k:f:t:vsiVe")) > -1 )
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
	    ctx.msg_verbosity++;
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
	}

    /* Get the crypto key unless given already */
    if (! key)
	key = getenv("FE_KEY");
    if (! key)
	key = getpass("fe key: ");
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
	buffer[0] = 0;
	for (i = optind; i < argc; i++) {
	    if (strlen(buffer) + strlen(argv[i]) + 1 >= sizeof(buffer))
		fe_error("Command too long, exceeds %d bytes\n",
		      (int)sizeof(buffer));
	    strcat(buffer, argv[i]);
	    if (i < argc - 1)
		strcat(buffer, " ");
	}
	fe_msg(&ctx, "About to run command: %s\n", buffer);
	ret = system(buffer);
    }

    return ret;
}
	       
