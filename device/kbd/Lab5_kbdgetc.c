/* Lab5_kbd_getc.c - kbd_getc */

#include <xinu.h>

devcall kbd_getc(struct dentry *devptr) {
    /* Wait for a character in the buffer and extract one character	*/
    wait(kbdcb.tyisem);
    char ch = *kbdcb.tyihead++;
    /* Wrap around to beginning of buffer, if needed */
    if (kbdcb.tyihead >= &kbdcb.tyibuff[TY_IBUFLEN]) {
        kbdcb.tyihead = kbdcb.tyibuff;
    }
    return (devcall) ch;
}