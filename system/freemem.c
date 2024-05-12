/* freemem.c - freemem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  freemem  -  Free a memory block, returning the block to the free list
 *------------------------------------------------------------------------
 */
syscall	freemem(
	  char		*addr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/

	mask = disable();
    /* Lab4 2021201780:Begin*/
	if ((nbytes == 0) || ((uint32) addr < (uint32) minheap)
			  || ((uint32) addr > (uint32) maxheap)) {
		restore(mask);
		return SYSERR;
	}
    /* Lab4 2021201780:End*/

    /* Lab4 2021201780:Begin*/
	nbytes = roundpg(nbytes);		/* Use psize multiples	*/
	uint32 npages = nbytes / PAGE_SIZE;

	for(int i = 0; i < npages; i++) {
		char* page_log = addr + i * PAGE_SIZE;
		pfree(fill_pgentry(page_log, 0, 0, TRUE));
		invlpg((void *)page_log);
	}
	/* Lab4 2021201780:End*/
	restore(mask);
	return OK;
}
