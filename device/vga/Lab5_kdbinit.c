/* Lab5_vgainit.c - vgainit */

#include <xinu.h>

struct kbdcblk kbdcb;

devcall vgainit() {
    set_evec(0x21, (uint32) vgadispatch, 0);
    kbdcb.tyihead = kbdcb.tyitail = kbdcb.tyibuff;
    kbdcb.tyisem = semcreate(0);
    kbdcb.tyicursor = 0;

    vga_init();
}