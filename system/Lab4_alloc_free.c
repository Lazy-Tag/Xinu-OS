/* alloc_free.c - alloc and free on heap */
#include <xinu.h>

uint32 palloc() {
    intmask mask;
    uint32 ret;

    mask = disable();
    ret = rec2phpg(freelist);    /* Get the first free page	*/

    if (ret == 0) {
        panic("[Error] No free page available.");
        return SYSERR;
    }

    freelist = (uint32 * ) * freelist;
    restore(mask);
    return ret;
}


syscall pfree(uint32 phy_addr) {
    intmask mask;

    mask = disable();

    uint32 *rec_addr = (uint32 *) ph2recpg(phy_addr);
    *rec_addr = (uint32) freelist;
    freelist = rec_addr;

    restore(mask);
    return OK;
}