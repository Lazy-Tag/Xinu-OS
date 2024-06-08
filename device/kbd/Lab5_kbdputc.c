/* Lab5 kbd_putc.c - kbd_putc */

#include <xinu.h>

devcall kbd_putc(struct dentry *devptr, char ch) {
    if (ch == TY_NEWLINE) {
        kbd_putc(devptr, TY_RETURN);
    }
    vga_putc(ch, FALSE);
    return OK;
}