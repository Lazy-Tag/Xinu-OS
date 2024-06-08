/* Lab5_kbd_init.c - kbd_init */

#include <xinu.h>

struct kbdcblk kbdcb;

devcall kbd_init() {
    set_evec(0x21, (uint32) kbd_dispatch, 0);

    kbdcb.tyihead = kbdcb.tyitail = kbdcb.tyibuff;
    kbdcb.tyisem = semcreate(0);
    kbdcb.tyicursor = 0;

    vga_init();
}