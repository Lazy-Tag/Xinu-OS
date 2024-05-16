/* alloc_free.c - alloc and free on heap */
#include <xinu.h>

uint32 physical_addr_alloc() {
    intmask mask;
    uint32 ret;

    mask = disable();
    ret = rec2phpg(freelist);    /* Get the first free page	*/

    if (ret == 0) {
        panic("[Error] No free page available.");
        return SYSERR;
    }
//    kprintf("Alloc physical address of new page at 0x%x\n", ret);

    // Update the freelist
    freelist = (uint32 * ) * freelist;
    restore(mask);
    return ret;
}


syscall physical_addr_free(uint32 phy_addr) {
    intmask mask;

    mask = disable();

    // Add the page to the freelist
    uint32 *rec_addr = (uint32 *) ph2recpg(phy_addr);
    *rec_addr = (uint32) freelist;
//    kprintf("Free physical address of free page at 0x%x\n", phy_addr);
    freelist = rec_addr;

    restore(mask);
    return OK;
}