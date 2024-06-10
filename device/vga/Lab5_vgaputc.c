/* Lab5 vgaputc.c - vgaputc */

#include <xinu.h>

devcall vgaputc(struct dentry *devptr, char ch) {
    if (ch == TY_NEWLINE) {
        vgaputc(devptr, TY_RETURN); // Write a return character
    }
    vga_putc(ch, FALSE);    // Write the character to VGA
    return OK;
}