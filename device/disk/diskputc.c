/* diskputc.c - diskputc */

#include <xinu.h>

/*------------------------------------------------------------------------
 * diskgetc  -  Read one character from a disk device
 *------------------------------------------------------------------------
 */
devcall	diskputc(
	  struct dentry	*devptr,		/* Entry in device switch table	*/
      char ch      /* Character to write */
	)
{
	struct	diskcblk	*diskptr;

	diskptr = &disktab[devptr->dvminor];
    uint32 addr = diskptr->lba_addr;
    uint32  sector = addr >> 9, tmp;

    if (sector != diskptr->prev_sector) {
        tmp = (sector >> 24) & 0xF;  /* bit 24-27 */
        outb(0x01F6, tmp | 0xE0);

        tmp = (sector >> 16) & 0xFF;  /* bit 16-23 */
        outb(0x01F5, tmp);

        tmp = (sector >> 8) & 0xFF;   /* bit 8-15 */
        outb(0x01F4, tmp);

        tmp = sector & 0xFF;          /* bit 0-7 */
        outb(0x01F3, tmp);

        outb(0x01F2, 1);              /* number of sectors */
        while (inb(0x01F7) & (1 << 7));

        outb(0x01F7, 0x20);           /* Command port for read */

        while (TRUE) {                /* wait until ready */
            tmp = inb(0x01F7);
            if (tmp & 8) {
                break;
            }
        }

        if (diskptr->prev_sector != -1)
            diskflush(devptr);
        insw(0x1F0, (int32)diskptr->cache, 256);
        diskptr->prev_sector = sector;
    }

    /* Update the cache with the new data */
    *((char *)diskptr->cache + (addr & 0x1FF)) = ch;
	diskptr->lba_addr++;
    diskptr->is_dirty = TRUE;
	return (devcall)OK;
}




