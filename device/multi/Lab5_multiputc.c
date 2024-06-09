#include <xinu.h>

/*------------------------------------------------------------------------
 *  multiputc  -  Write one character to a tty device and a VGA device
 *------------------------------------------------------------------------
 */

devcall multiputc(struct dentry *devptr, char ch) {
    return vgaputc(devptr, ch) && ttyputc(devptr, ch);
}
