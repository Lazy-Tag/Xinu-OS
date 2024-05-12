/* log2ph.c - log_addr to phy_addr for page */
#include <xinu.h>

uint32	log2ph(char	*addr)
{
	intmask 	mask;
	uint32		ret;

	mask = disable();

	uint32 dir = (uint32)addr >> 22;
	pgtab	*pgtable = (pgtab *)(0x00800000 | (dir << 12));
	uint32 table = ((uint32)addr >> 12) & 0x3FF;
	uint32 offset = (uint32)addr & 0xFFF;

	if ((pgdir->entry[dir] & PT_ENTRY_P) == 0) {
		restore(mask);
		return SYSERR;
	}

	if ((pgtable->entry[table] & PT_ENTRY_P) == 0) {
		restore(mask);
		return SYSERR;
	}
	ret = getaddr(pgtable->entry[table]) | offset;

	restore(mask);
	return ret;
}
