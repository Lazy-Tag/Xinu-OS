/* disk.c */

#include <xinu.h>

struct	diskcblk	disktab[Ndisk];

void diskinit() {
    int i;
    for (i = 0; i < Ndisk; i++) {
        disktab[i].prev_sector = -1;
        disktab[i].lba_addr = 0;
        disktab[i].is_dirty = FALSE;
    }
}