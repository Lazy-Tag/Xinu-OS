/* Lab5 kbd_read.c - kbd_read */

#include <xinu.h>

devcall kbd_read(struct dentry *devptr, char *buff, int32 count) {
    if (count < 0) {
        return SYSERR;
    }

    /* Block until input arrives */
    int firstch = kbd_getc(devptr);
    if (firstch == EOF) {
        return EOF;
    }
    /* Read up to a line */
    char ch = (char) firstch;
    *buff++ = ch;
    int nread = 1;
    while ((nread < count) &&
           ch != TY_NEWLINE && ch != TY_RETURN) {
        ch = kbd_getc(devptr);
        *buff++ = ch;
        nread++;
    }
    return nread;
}