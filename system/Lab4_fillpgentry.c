/* fill_page_entry.c - fill or clear the page table entry */

#include <xinu.h>

int	isfree(char		*log_addr) {
	intmask 	mask;
	mask = disable();

	if ((uint32)log_addr % PAGE_SIZE != 0 ||
	    (uint32)log_addr < (uint32)minheap || (uint32)log_addr >= (uint32)maxheap) {
		restore(mask);
		return FALSE;
	}

	uint32 dir = (uint32)log_addr >> 22;
	pgtab *pgtable = (pgtab *)(0x00800000 | (dir << 12));
	uint32 table = ((uint32)log_addr >> 12) & 0x3FF;

    // Check if the page dir entry is not present
	if ((pgdir->entry[dir] & PT_ENTRY_P) == 0) {
		restore(mask);
		return TRUE;
	}

    // Check if the page table entry is not present
	if ((pgtable->entry[table] & PT_ENTRY_P) == 0) {
		restore(mask);
		return TRUE;
	}

	restore(mask);
	return FALSE;
}

uint32	fill_page_entry(
	  char			*logical_addr, // The logical address to fill
	  uint32		physical_addr, // The physical address to map
	  uint32		ptmask,
	  int			is_free // If the page is free
	)
{
	intmask 	mask;
	uint32		ret;

	mask = disable();

	uint32 dir = (uint32)logical_addr >> 22;
	pgtab *pgtable = (pgtab *)(0x00800000 | (dir << 12));
	uint32 table = ((uint32)logical_addr >> 12) & 0x3FF;

    // Check if the page dir entry is not present
	if ((pgdir->entry[dir] & PT_ENTRY_P) == 0) {
        if (is_free) {
			restore(mask);
			return SYSERR;
		}
        // Allocate a new page table
		uint32 tmp_addr = physical_addr_alloc();
		pgdir->entry[dir] = tmp_addr | PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U;
		memset((void *)pgtable, 0, PAGE_SIZE);
	}
	ret = getaddr(pgtable->entry[table]);

    // Check if the page table entry is not present and the page is trying to free
	if (is_free && ((pgtable->entry[table] & PT_ENTRY_P) == 0)) {
		restore(mask);
		return SYSERR;
	}

	pgtable->entry[table] = physical_addr | ptmask;

	restore(mask);
	return ret;
}