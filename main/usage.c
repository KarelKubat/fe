#include "../fe.h"

void usage(void) {
    printf(
"\n"
"Welcome to FE V" VER ", a file-based encryptor.\n"
"Copyright (c) Karel Kubat 2015ff, all rights reserved.\n"
"Contact <karel@kubat.nl> for information. Distributed under GPLV3.\n"
"\n"
"Usage: fe [-h?vsei -k KEY] [-t TARGET ] command arguments\n"
"   Or: fe [-h?vsei -k KEY]  -f FILE\n"	    
"Supported flags are:\n"
"  -h, -?    This help\n"
"  -v        Increases verbosity\n"
"  -s        Sends verbose messages to stderr instead of to syslog\n"
"  -e        Uses the environment to pass context to next programs,\n"
"            default: shared memory\n"
"  -i        Ignores non-caught calls such as fscanf instead of aborting.\n"
"            With -i, fprintf/fscanf will not transcrypt but will work.\n"
"  -k KEY    Initializes the crypto functions with the stated key. When this\n"
"            flag is absent, the FE_KEY environment variable is used. When\n"
"            neither -k is given nor FE_KEY is present, the key is read from\n"
"            stdin.\n"
"  -t FILE   Marks the file as transcryption target in a following command.\n"
"            Default is to transcrypt all files within the arguments. Flag is\n"
"            repeatable to specify more targets.\n"
"  -f FILE   Transcrypts the stated file\n"
"  -V        prints the version ID on stdout and stops\n"
"\n"
"When the command is given, then it is invoked with fe as a layer underneath\n"
"to transcrypt file(s) that the command opens.\n"
"\n");
    exit(1);
}
