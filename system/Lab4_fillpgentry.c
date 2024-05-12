/* fill_pgentry.c - fill or clear the page table entry */

#include <xinu.h>

int	isfree(char		*lg_addr) {
	intmask 	mask;
	mask = disable();

	if ((uint32)lg_addr % PAGE_SIZE != 0 ||
	    (uint32)lg_addr < (uint32)minheap || (uint32)lg_addr >= (uint32)maxheap) {
		restore(mask);
		return FALSE;
	}

	uint32 dir = (uint32)lg_addr >> 22;
	pgtab *pgtable = (pgtab *)(0x00800000 | (dir << 12));
	uint32 table = ((uint32)lg_addr >> 12) & 0x3FF;

	if ((pgdir->entry[dir] & PT_ENTRY_P) == 0) {
		restore(mask);
		return TRUE;
	}

	if ((pgtable->entry[table] & PT_ENTRY_P) == 0) {
		restore(mask);
		return TRUE;
	}

	restore(mask);
	return FALSE;
}

uint32	fill_pgentry(
	  char			*logical,
	  uint32		physical,
	  uint32		ptmask,
	  int			is_free
	)
{
	intmask 	mask;
	uint32		ret;

	mask = disable();

	uint32 dir = (uint32)logical >> 22;
	pgtab *pgtable = (pgtab *)(0x00800000 | (dir << 12));
	uint32 table = ((uint32)logical >> 12) & 0x3FF;

	if ((pgdir->entry[dir] & PT_ENTRY_P) == 0) {
		if (is_free) {
			restore(mask);
			return SYSERR;
		}
		uint32 tmp_addr = palloc();
		pgdir->entry[dir] = tmp_addr | PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U;
		memset((void *)pgtable, 0, PAGE_SIZE);
	}
	ret = getaddr(pgtable->entry[table]);

	if (is_free && ((pgtable->entry[table] & PT_ENTRY_P) == 0)) {
		restore(mask);
		return SYSERR;
	}

	pgtable->entry[table] = physical | ptmask;

	restore(mask);
	return ret;
}