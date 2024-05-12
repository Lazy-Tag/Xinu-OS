/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct procent *prptr;		/* Ptr to process's table entry	*/
    // Lab4 2021201780
	int32	i, j;			/* Index into descriptors	*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}

    /* Lab4 2021201780:begin */
    /* Free stack, heap and page table of this process */
	pgtab *cur_pgtable;
	/* Heap and user stack */
	for (i = 3; i < PT_NENTRY; i++) {
		if ((pgdir->entry[i] & PT_ENTRY_P) == 0) continue;
		cur_pgtable = (pgtab *)(0x00800000 + (i << 12));
		for (j = 0; j < PT_NENTRY; j++) {
			if (cur_pgtable->entry[j] & PT_ENTRY_P) {
				pfree(getaddr(cur_pgtable->entry[j]));
			}
		}
		pfree(getaddr(pgdir->entry[i]));
	}
	/* Kernel stack */
	j = (uint32)&end / PAGE_SIZE + 1;
	cur_pgtable = (pgtab *)0x00800000;
	pfree(getaddr(cur_pgtable->entry[j]));
	pfree(getaddr(pgdir->entry[0]));
	pfree(prptr->prpgdir);
    /* Lab4 2021201780:end */

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}
