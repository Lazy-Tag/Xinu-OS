#include <xinu.h>

/*------------------------------------------------------------------------
 *  multiinit  -  Initialize buffers and modes for a multi line
 *------------------------------------------------------------------------
 */

devcall multiinit(struct dentry *devptr) {
    // Initialize both kbdvga and tty devices
    vgainit();
    ttyinit(devptr);
    set_evec(0x21, (uint32) multidispatch, 0);
    return OK;
}
