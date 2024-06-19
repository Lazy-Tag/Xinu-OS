/* diskflush.c - diskflush */

#include <xinu.h>

/*------------------------------------------------------------------------
 * diskflush -  Flush the cache of a disk device
 *------------------------------------------------------------------------
 */
devcall	diskflush (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	struct	diskcblk	*diskptr;

	diskptr = &disktab[devptr->dvminor];
    uint32  sector = disktab->prev_sector, tmp;

    if (!diskptr->is_dirty)
        return (devcall)OK;
    diskptr->is_dirty = FALSE;

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

    outb(0x01F7, 0x30);           /* Command port for write */

    while (TRUE) {                /* wait until ready */
        tmp = inb(0x01F7);
        if (tmp & 8) {
            break;
        }
    }

    outsw(0x1F0, (int32)diskptr->cache, 256);

	return (devcall)OK;
}
