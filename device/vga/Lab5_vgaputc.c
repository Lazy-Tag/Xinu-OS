/* Lab5 vgaputc.c - vgaputc */

#include <xinu.h>

devcall vgaputc(struct dentry *devptr, char ch) {
    if (ch == TY_NEWLINE) {
        vgaputc(devptr, TY_RETURN);
    }
    vga_putc(ch, FALSE);
    return OK;
}