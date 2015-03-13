
/* Generic includes */
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
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

/* Runtime context definition */

/* Verbosity estination: stderr or syslog? */
typedef enum {
    dst_syslog,
    dst_stderr,
} MsgDst;

/* File target definition */
typedef struct {
    char *name;
    dev_t st_dev;
    ino_t st_ino;
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
    uint32_t seed;
} FeCtx;

/* Local functions */
extern void cryptbuf(char *buf, size_t bufsz, size_t offset);
extern void cryptfile(char const *f);
extern void *dllookup(char const *name);
extern void error(char const *fmt, ...) __attribute__((format(printf, 1, 2)));
extern FeCtx *fectx (void);
extern int is_fd_target(int fd);
extern int isfile(struct stat const *st);
extern void msg(FeCtx const *ctx, char const *fmt, ...)
    __attribute__((format(printf, 2, 3)));
extern char randbyte(FeCtx *ctx, uint32_t x);
extern void randinit(FeCtx *ctx, char const *s);
extern char *readkey(void);
extern void target_add(FeCtx *ctx, char const *name);
extern FeCtx *target_deserialize(char const *s);
extern void targets_msg(FeCtx *ctx);
extern char *target_serialize(FeCtx const *ctx);
extern void target_update_fd(char const *name, int fd);
extern void *xmalloc(size_t sz);
extern void *xrealloc(void *mem, size_t newsz);
extern char *xstrcat(char *mem, char const *s);
extern char *xstrdup(char const *s);

