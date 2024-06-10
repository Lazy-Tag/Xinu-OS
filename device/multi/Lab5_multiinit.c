#include <xinu.h>

/*------------------------------------------------------------------------
 *  multiinit  -  Initialize buffers and modes for a multi line
 *------------------------------------------------------------------------
 */

devcall multiinit(struct dentry *devptr) {
    // Initialize both kbdvga and tty devices
    vgainit();
    ttyinit(devptr);
    return OK;
}
