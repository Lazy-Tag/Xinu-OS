#include <xinu.h>

void _do_stk_expand(uint32 addr)
{
    kprintf("Do stk expand...\n");
    pid32 pid = getpid();
    struct procent *prptr = &proctab[pid];
    uint32 dir = (uint32)addr >> 22;
	pgtab	*pgtable = (pgtab *)(0x00800000 | (dir << 12));
    if (((uint32) 0xFE000000 - addr) > (1 << 22))
        panic("Invalid page access!");

    uint32 oldstklen = prptr->prstklen;
    uint32 newstklen = roundpg((uint32) prptr->uprstkbase - addr);
    uint32 npages = (newstklen - oldstklen) / PAGE_SIZE;
    char *old_log = (char *)getmem(newstklen - oldstklen), *new_log = maxheap;
    new_log -= newstklen;

    for (int i = 0; i < npages; i++) {
		uint32 table = ((uint32)new_log >> 12) & 0x3FF;
		uint32 page_addr = log2ph(old_log);
		pgtable->entry[table] = page_addr | PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U;
		old_log += PAGE_SIZE;
		new_log += PAGE_SIZE;
	}
    prptr->prstklen = newstklen;
}
