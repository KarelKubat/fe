#include "../fe.h"

char *fe_printable_bytes(unsigned char const *bufp, size_t n) {
    char *pr = 0, conv[10];
    size_t i;

    for (i = 0; i < n; i++, bufp++)
        switch (*bufp) {
        case '\a':
            pr = fe_xstrcat(pr, "\\a");
            break;
        case '\b':
            pr = fe_xstrcat(pr, "\\b");
            break;
        case '\n':
            pr = fe_xstrcat(pr, "\\n");
            break;
        case '\r':
            pr = fe_xstrcat(pr, "\\r");
            break;
        case '\t':
            pr = fe_xstrcat(pr, "\\t");
            break;
        case '\v':
            pr = fe_xstrcat(pr, "\\v");
            break;
        default:
            if (isprint(*bufp)) {
                conv[0] = *bufp;
                conv[1] = 0;
            } else
                sprintf(conv, "\\%3.3o", *bufp & 0xff);
            pr = fe_xstrcat(pr, conv);
            break;
        }

    return pr;
}
