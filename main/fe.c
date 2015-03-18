#include "../fe.h"

int main(int argc, char **argv) {
    int opt, target_set = 0, i;
    char *file_to_crypt = 0, buffer[1024], *cp, *key = 0;
    static FeCtx ctx;

    /* Catchall usage info */
    if (argc == 1)
	usage();

    /* Parse command line */
    while ( (opt = getopt(argc, argv, "h?k:f:t:vs")) > -1 )
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
	    target_add(&ctx, optarg);
	    target_set++;
	    break;
	case 'v':
	    ctx.msg_verbosity++;
	    break;
	case 's':
	    ctx.msg_dst = dst_stderr;
	    break;
	}

    /* Get the crypto key unless given already */
    if (! key)
	key = getenv("FE_KEY");
    if (! key)
	key = getpass("fe key: ");
    randinit(&ctx, key);

    /* Find targets, if not done so yet via flags -t */
    if (! target_set)
	for (i = optind + 1; i < argc; i++)
	    target_add(&ctx, argv[i]);
    targets_msg(&ctx);

    /* Set FE environment */
    cp = fectx_serialize(&ctx);
    fe_msg(&ctx, "Serialized context: %s\n", cp);
    setenv("FE_CTX", cp, 1);

    if (file_to_crypt) {
	if (optind != argc)
	    fe_error("No command and arguments accepted with flag -f\n");
	cryptfile(file_to_crypt);
    } else if (optind == argc) {
	fe_error("No action taken, try 'fe -h' for an overview\n");
    } else {
	/* Going to launch the command with the shared object underneath */
	/* For MacOSX: */
	setenv("DYLD_INSERT_LIBRARIES", LIBDIR "/" LIB, 1);
	setenv("DYLD_FORCE_FLAT_NAMESPACE", "1", 1);
	/* For Linux: */
	setenv("LD_PRELOAD", LIBDIR "/" LIB, 1);

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
	system(buffer);
    }

    return 0;
}
	       
