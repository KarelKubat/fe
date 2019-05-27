#include "../fe.h"

/* Local functions for the main program, undeclared in fe.h */
extern void cryptfile(char const *f);
extern char *quoted(char const *s);
extern void usage(void);

int main(int argc, char **argv, char **envp) {
  int opt, target_set = 0, i, ret = 0, dump_env = 0, beep = 0;
  char *file_to_crypt = 0, buffer[1024], *sysbuf, *key = 0, *key_again, *cp;
  static FeCtx ctx;

  /* Long options to parse from the cmd line */
  static struct option longopts[] =
    {
     { "help",             no_argument,       0, 'h' },
     { "key",              required_argument, 0, 'k' },
     { "stderr-log",       no_argument,       0, 's' },
     { "dump-environment", no_argument,       0, 'E' },
     { "environment",      no_argument,       0, 'e' },
     { "ignore-uncaught",  no_argument,       0, 'i' },
     { "debug",            no_argument,       0, 'd' },
     { "target",           required_argument, 0, 't' },
     { "file",             required_argument, 0, 'f' },
     { "verbose",          no_argument,       0, 'v' },
     { "version",          no_argument,       0, 'V' },
     { "beep",             no_argument,       0, 'b' },
     { 0,		   0,		      0,  0  }
    };

  /* Catchall usage info */
  if (argc == 1)
    usage();

  /* Parse command line */
  while ( (opt = getopt_long(argc, argv, "h?a:k:f:t:vsiVEed",
			     longopts, 0)) > -1 )
    switch (opt) {
    case 'h':
    case '?':
      usage();
    case 'k':
      key = optarg;
      break;
    case 's':
      ctx.msg_dst = dst_stderr;
      break;
    case 'E':
      dump_env = 1;
      break;
    case 'e':
      ctx.use_env = 1;
      break;
    case 'i':
      ctx.ignore_noncaught = 1;
      break;
    case 'd':
      ctx.debug = 1;
      ctx.msg_verbosity = 1;
      break;
    case 't':
      fe_target_add(&ctx, optarg);
      target_set++;
      break;
    case 'f':
      file_to_crypt = optarg;
      break;
    case 'v':
      ctx.msg_verbosity = 1;
      break;
    case 'V':
      printf(VER "\n");
      return 0;
    case 'b':
      beep = 1;
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
      cp = fgets(buffer, sizeof(buffer) - 1, stdin);
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

  if (file_to_crypt) {
    if (optind != argc)
      fe_error("No command and arguments accepted with flag -f\n");
    /* Ignore signals that we can ignore, we don't want to stop while
     * directly (en)crypting a file. Don't set signals in passthru mode.
     */
    signal(SIGHUP,  SIG_IGN);
    signal(SIGINT,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    cryptfile(file_to_crypt);
  } else if (optind == argc) {
    fe_error("No action taken, try 'fe -h' for an overview\n");
  } else {
    /* Going to launch the command with the shared object underneath */

#if USYS == UN_DARWIN
    /* For MacOSX: This unfortunately works only for selected programs.
     * E.g. stuff from /usr/local/bin/ will obey loaded libs. but standard
     * /usr/bin and /bin (such as cp) will happily ignore. */
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
      free(q);
      if (i < argc - 1)
	sysbuf = fe_xstrcat(sysbuf, " ");
    }
    fe_msg(&ctx, "About to run command: %s\n", sysbuf);
    if (dump_env) {
      char **e;
      fe_msg(&ctx, "Environment dump:\n");
      for (e = envp; *e; e++)
	fe_msg(&ctx, "%s\n", *e);
    }

    if ( (ret = system(sysbuf)) == -1 )
      fe_error("Failed to run command: cannot fork\n");
    else if (ret == 127)
      fe_error("Failed to run command: shell failure\n");
  }

  if (beep && isatty(1))
    for (i = 0; i < 5; ++i) {
      putchar('\a');
      sleep(1);
    }

  return ret;
}
