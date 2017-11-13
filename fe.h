
/* Generic includes */
#define __USE_GNU
#define _GNU_SOURCE
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#ifdef HAVE_LIBIO_H_
#include <libio.h>
#endif
#include <libgen.h>
#include <pwd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

/* Supported systems */
#define UN_DARWIN 1
#define UN_LINUX  2

/* Local includes */
#include "lib/SHA3api_ref.h"

/* Runtime context definition */

/* Verbosity estination: stderr or syslog? */
typedef enum {
    dst_syslog,
    dst_stderr,
} MsgDst;

/* File target definition */
typedef struct {
    char *name;
    int fd;
} Target;

typedef struct {
    /* Messaging */
    int msg_verbosity;
    MsgDst msg_dst;
    /* Ignore noncaught libc calls or not */
    int ignore_noncaught;
    /* File targets */
    int ntargets;
    Target *targets;
    /* Random generator related */
    char *seed;
    /* Pass via environment or via shared memory? */
    int use_env;
    /* Debugging: fe won't transcrypt, but will pass through all caught
     * libc functions */
    int debug;
} FeCtx;

/* Check that MAGIC is defined, which should be kept in Makefile.local */
#ifndef MAGIC
#error "You need to define a MAGIC string for the build. RTFM please."
#endif

/* Hashing-related */
#define HASH_BIT_SIZE   1024
#define HASH_BYTE_SIZE  (HASH_BIT_SIZE / 8)

/* Shared lib functions */
extern void fe_close_fd(int fd);
extern void fe_cryptbuf(char *buf, size_t bufsz, size_t offset,
			BitSequence *hashval, int algorithm);
extern void *fe_dllookup(char const *name);
extern void fe_error(char const *fmt, ...)
    __attribute__((format(printf, 1, 2)));
extern int fe_is_fd_target(int fd);
extern int fe_isfile(struct stat const *st);
extern void fe_msg(FeCtx const *ctx, char const *fmt, ...)
    __attribute__((format(printf, 2, 3)));
extern char fe_randbyte(FeCtx *ctx, uint32_t x,
                        BitSequence *hashval, int algorithm);
extern char fe_randbyte_keyed(char const *key, uint32_t x,
			      BitSequence *hashval, int algorithm);
extern char *fe_seed_deserialize(char const *buf);
extern char *fe_seed_serialize(char const *buf);
extern FeCtx *fe_setup(char const *key, int verbosity, MsgDst dst,
		       int ignore_uncaught, int use_env, int debug);
extern void *fe_shmat(int shmid);
extern void fe_target_add(FeCtx *ctx, char const *name);
extern void fe_target_by_fd(int oldfd, int newfd);
extern void fe_target_by_path(char const *name, int newfd);
extern int fe_xasprintf(char **ret, const char *fmt, ...);
extern char *fe_xdirname(char const *t);
extern char *fe_xbasename(char const *t);
extern void *fe_xmalloc(size_t sz);
extern char *fe_xstrdup(char const *s);
extern char *fe_xstrcat(char *mem, char const *s);
extern void *fe_xrealloc(void *mem, size_t newsz);
extern char *fe_xrealpath(char const *path);
extern int fe_xvasprintf(char **ret, char const *fmt, va_list args);

extern FeCtx *fectx (void);
extern FeCtx *fectx_deserialize(char const *s);
extern char *fectx_serialize(FeCtx const *ctx);
extern void fectx_set(FeCtx const *ctx);
extern void fectx_unset(void);
