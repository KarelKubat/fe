/*
 * Main fe program
 */

/* [KK 2015-03-06] Initial version */
#define VER "1.00"

#include "../fe.h"

FeCtx ctx;

static void usage(void);

int main(int argc, char **argv) {
    int opt, key_initialized = 0, target_set = 0, i;
    char *file_to_crypt = 0;
    char buffer[1024], *cp;

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
	    if (!strcmp(optarg, "-")) {
		randinit(&ctx, readkey());
		msg(&ctx, "Cryptokey read from stdin.\n");
	    }
	    else {
		randinit(&ctx, optarg);
		msg(&ctx, "Cryptokey obtained from command line.\n");
	    }
	    key_initialized = 1;
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
    if (! key_initialized) {
	char *key = getenv("FE_KEY");
	if (!key)
	    error("Neither an environment variable FE_KEY is set, "
		  "nor flag -k used to initialize crypto, try 'fe -h' "
		  "for an overview\n");
	randinit(&ctx, key);
	msg(&ctx, "Cryptokey obtained from environment.\n");
    }

    /* Find targets, if not done so yet via flags -t */
    if (! target_set)
	for (i = optind + 1; i < argc; i++)
	    target_add(&ctx, argv[i]);

    /* Set FE environment */
    cp = target_serialize(&ctx);
    msg(&ctx, "Serialized context: %s\n", cp);
    setenv("FE_CTX", cp, 1);

    if (file_to_crypt) {
	if (optind != argc)
	    error("No command and arguments accepted with flag -f\n");
	cryptfile(file_to_crypt);
    } else if (optind == argc) {
	error("No action taken, try 'fe -h' for an overview\n");
    } else {
	/* Going to launch the .so. Set targets if not yet done. */
	targets_msg(&ctx);

	setenv("DYLD_INSERT_LIBRARIES", "/usr/local/lib/libfe.dylib", 1);
	setenv("DYLD_FORCE_FLAT_NAMESPACE", "1", 1);

	/* Run the intended command */
	buffer[0] = 0;
	for (i = optind; i < argc; i++) {
	    if (strlen(buffer) + strlen(argv[i]) + 1 >= sizeof(buffer))
		error("Command too long, exceeds %d bytes\n",
		      (int)sizeof(buffer));
	    strcat(buffer, argv[i]);
	    if (i < argc - 1)
		strcat(buffer, " ");
	}
	msg(&ctx, "About to run command: %s\n", buffer);
	system(buffer);
    }

    return 0;
}
	       
static void usage(void) {
    fprintf(stderr,
"\n"
"Welcome to FE V" VER ", a file-based encryptor.\n"
"Copyright (c) Karel Kubat 2015ff, all rights reserved.\n"
"Contact <karel@kubat.nl> for information. Distributed under GPLV3.\n"
"\n"
"Usage: fe [-h -? -k KEY -v -s] [-t TARGET ] command arguments\n"
"   Or: fe [-h -? -k KEY -v -s]  -f FILE\n"	    
"Supported flags are:\n"
"  -h, -?    This help\n"
"  -k KEY    Initializes the crypto functions with the stated key. When this\n"
"            flag is absent, the FE_KEY environment variable is used. When\n"
"            key is one hypen (as in -k-) then the key is read from stdin.\n"
"  -t FILE   Marks the file as transcryption target in a following command.\n"
"            Default is to transcrypt all files within the arguments. Flag is\n"
"            repeatable to specify more targets.\n"
"  -f FILE   Transcrypts the stated file\n"
"  -v        Increases verbosity\n"
"  -s        Sends verbose messages to stderr instead of to syslog\n"	    
"\n"
"When the command is given, then it is invoked with fe as a layer underneath\n"
"to transcrypt file(s) that the command opens.\n"
"\n");
    exit(1);
}
