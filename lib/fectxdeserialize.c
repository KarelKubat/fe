#include "../fe.h"

FeCtx *fectx_deserialize(char const *s) {
    static FeCtx ctx;
    static int deserialized = 0;    
    int i;
    char b2[2] = {0, 0};

    /* Parse just once */
    if (deserialized)
	return &ctx;
    deserialized++;

    if (*s == '1')
	ctx.msg_verbosity = 1;
    else if (*s == '0')
	ctx.msg_verbosity = 0;
    else
	error("Bad FE_CTX setting at %s\n", s);
    s++;

    if (*s == 'l')
	ctx.msg_dst = dst_syslog;
    else if (*s == 'e')
	ctx.msg_dst = dst_stderr;
    else
	error("Bad FE_CTX setting at %s\n", s);
    s++;

    sscanf(s, "%d", &ctx.ntargets);
    while (isdigit(*s))
	s++;
    s++;
    if (ctx.ntargets) {
	ctx.targets = xmalloc(ctx.ntargets * sizeof(Target));
	for (i = 0; i < ctx.ntargets; i++) {
	    ctx.targets[i].name = xstrdup("");
	    while (*s != ':') {
		b2[0] = *s;
		ctx.targets[i].name = xstrcat(ctx.targets[i].name, b2);
		s++;
	    }
	    s++;
	}
    }
    ctx.seed = xstrdup(seed_deserialize(s));

    return &ctx;
}
	
	
