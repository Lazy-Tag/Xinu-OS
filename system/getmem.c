/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

    /*Lab4 2021201780: begin */
	nbytes = (uint32) roundpg(nbytes);	/* Use memblk multiples	*/

	uint32 npages = nbytes / PAGE_SIZE;

	char* mem_begin = getheap(nbytes);
	// now get continuous pages

	if (mem_begin != (char *)SYSERR) {
		for (int i = 0; i < npages; i++) {
			char* page_log = mem_begin + i * PAGE_SIZE;
			uint32 page_phy = palloc();
			fill_pgentry(page_log, page_phy, PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U, FALSE);
		}
	}

	restore(mask);
	return mem_begin;
    /*Lab4 2021201780: end */
}
