
/* Generic includes */
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>

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
    /* File targets */
    int ntargets;
    Target *targets;
    /* Random generator related */
    char *seed;
} FeCtx;

/* Check that MAGIC is defined, which should be kept in Makefile.local */
#ifndef MAGIC
#error "You need to define a MAGIC string for the build. RTFM please."
#endif

/* Hashing-related */
#define HASH_BIT_SIZE   1024
#define HASH_BYTE_SIZE  (HASH_BIT_SIZE / 8)

/* Local functions */
extern void cryptbuf(char *buf, size_t bufsz, size_t offset,
		     BitSequence *hashval);
extern void cryptfile(char const *f);
extern void *dllookup(char const *name);
extern void error(char const *fmt, ...) __attribute__((format(printf, 1, 2)));
extern FeCtx *fectx (void);
extern FeCtx *fectx_deserialize(char const *s);
extern char *fectx_serialize(FeCtx const *ctx);
extern int is_fd_target(int fd);
extern int isfile(struct stat const *st);
extern void msg(FeCtx const *ctx, char const *fmt, ...)
    __attribute__((format(printf, 2, 3)));
extern char randbyte(FeCtx *ctx, uint32_t x, BitSequence *hashval);
extern char randbyte_keyed(char const *key, uint32_t x, BitSequence *hashval);
extern void randinit(FeCtx *ctx, char const *s);
extern char *seed_deserialize(char const *buf);
extern char *seed_serialize(char const *buf);
extern void target_add(FeCtx *ctx, char const *name);
extern void targets_msg(FeCtx *ctx);
extern void target_update_fd(char const *name, int fd);
extern int xasprintf(char **ret, const char *fmt, ...);
extern void *xmalloc(size_t sz);
extern void *xrealloc(void *mem, size_t newsz);
extern char *xrealpath(char const *path);
extern char *xstrcat(char *mem, char const *s);
extern char *xstrdup(char const *s);

