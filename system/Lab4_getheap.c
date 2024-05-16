/* getheap.c - find the free page on heap */
#include <xinu.h>

char *getheap(uint32 nbytes) {
    intmask mask;

    mask = disable();
    if (nbytes == 0) {
        restore(mask);
        return (char *) SYSERR;
    }

    nbytes = roundpg(nbytes);
    uint32 npages = nbytes / PAGE_SIZE;
    // Start from minheap
    char *addr_begin = (char *) 0x00C00000;

    // Find a free continuous page on heap by first-fit
    while ((uint32) addr_begin < 0xFE000000) {
        int i;
        for (i = 0; i < npages; i++) {
            uint32 page_begin = (uint32) addr_begin + i * PAGE_SIZE;
            if (!isfree((char *) page_begin))
                break;
        }
        if (i == npages) {
            restore(mask);
            return addr_begin;
        } else addr_begin += (i + 1) * PAGE_SIZE;
    }

    restore(mask);
    return (char *) SYSERR;
}
