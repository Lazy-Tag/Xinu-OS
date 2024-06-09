/* Lab5 vgaread.c - vgaread */

#include <xinu.h>

devcall vgaread(struct dentry *devptr, char *buff, int32 count) {
    if (count < 0) {
        return SYSERR;
    }

    /* Block until input arrives */
    int firstch = vgagetc(devptr);
    if (firstch == EOF) {
        return EOF;
    }
    /* Read up to a line */
    char ch = (char) firstch;
    *buff++ = ch;
    int nread = 1;
    while ((nread < count) &&
           ch != TY_NEWLINE && ch != TY_RETURN) {
        ch = vgagetc(devptr);
        *buff++ = ch;
        nread++;
    }
    return nread;
}