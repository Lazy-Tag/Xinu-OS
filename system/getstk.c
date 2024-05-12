/* getstk.c - getstk */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getstk  -  Allocate stack memory, returning highest word address
 *------------------------------------------------------------------------
 */
/* Lab4 2021201780:begin */
char *getstk(
        uint32 nbytes,
        pgtab *pgtable,
        int is_kernel
) {
    intmask mask;            /* Saved interrupt mask		*/

    mask = disable();
    if (nbytes == 0) {
        restore(mask);
        return (char *) SYSERR;
    }

    nbytes = roundpg(nbytes);
	uint32 npages = nbytes / PAGE_SIZE;

	char *old_log = getmem(nbytes) ,*new_log;
	if (is_kernel) new_log = (char *)&end + 2 * PAGE_SIZE;
	else new_log = (char *)maxheap;
	new_log -= nbytes;

	for (int i = 0; i < npages; i++) {
		uint32 table = ((uint32)new_log >> 12) & 0x3FF;
		uint32 page_addr = log2ph(old_log);
		pgtable->entry[table] = page_addr | PT_ENTRY_P | PT_ENTRY_W;
		if (!is_kernel) pgtable->entry[table] |= PT_ENTRY_U;
		old_log += PAGE_SIZE;
		new_log += PAGE_SIZE;
	}
	restore(mask);
	return old_log - sizeof(uint32);
}
/* Lab4 2021201780:end */
