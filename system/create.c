/* create.c - create, newpid */

#include <xinu.h>

local int newpid();

/*------------------------------------------------------------------------
 *  create  -  Create a process to start running a function on x86
 *------------------------------------------------------------------------
 */
pid32 create(
        void *funcaddr,    /* Address of the function	*/
        uint32 ssize,        /* Stack size in bytes		*/
        pri16 priority,    /* Process priority > 0		*/
        char *name,        /* Name (for debugging)		*/
        //Lab3 2021201780
        uint32 nargs,        /* Number of args that follow	*/
        ...
) {
    uint32 savsp, *pushsp;
    intmask mask;        /* Interrupt mask		*/
    pid32 pid;        /* Stores new process id	*/
    struct procent *prptr;        /* Pointer to proc. table entry */
    int32 i;
    uint32 *a;        /* Points to list of args	*/
    uint32 *saddr;        /* Stack address		*/
    //Lab3 2021201780
    uint32 *usaddr;   /* User Stack address		*/
    mask = disable();
    if (ssize < MINSTK)
        ssize = MINSTK;

    /*Lab4 2021201780:begin */
    ssize = (uint32) roundpg(ssize);
    if ((priority < 1) || ((pid = newpid()) == SYSERR)) {
        restore(mask);
        return SYSERR;
    }
    pgtab *newpgdir = (pgtab *) getmem(PAGE_SIZE);
    if (newpgdir == (pgtab *) SYSERR) {
        restore(mask);
        return SYSERR;
    }
    memset((void *) newpgdir, 0, PAGE_SIZE);

    pgtab *newpt0, *oldpt0, *newptx;
    newpt0 = (pgtab *) getmem(PAGE_SIZE);
    if (newpt0 == (pgtab *) SYSERR) {
        freemem((char *) newpgdir, PAGE_SIZE);
        restore(mask);
        return SYSERR;
    }
    oldpt0 = (pgtab *) 0x800000;
    for (i = 0; i < PT_NENTRY; ++i)
        newpt0->entry[i] = oldpt0->entry[i];
    newptx = (pgtab *) getmem(PAGE_SIZE);
    if (newptx == (pgtab *) SYSERR) {
        freemem((char *) newpgdir, PAGE_SIZE);
        freemem((char *) newpt0, PAGE_SIZE);
        restore(mask);
        return SYSERR;
    }
    memset((void *) newptx, 0, PAGE_SIZE);
    newpgdir->entry[0] = log2ph((char *) newpt0) | PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U;
    newpgdir->entry[1] = pgdir->entry[1];
    newpgdir->entry[2] = log2ph((char *) newpgdir) | PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U;
    newpgdir->entry[1015] = log2ph((char *) newptx) | PT_ENTRY_P | PT_ENTRY_W | PT_ENTRY_U;

    saddr = (uint32 *) getstk(KERNELSTK, newpt0, TRUE);
    if (saddr == (uint32 *) SYSERR) {
        freemem((char *) newpgdir, PAGE_SIZE);
        freemem((char *) newpt0, PAGE_SIZE);
        freemem((char *) newptx, PAGE_SIZE);
        restore(mask);
        return SYSERR;
    }
    usaddr = (uint32 *) getstk(ssize, newptx, FALSE);
    if (usaddr == (uint32 *) SYSERR) {
        freemem((char *) newpgdir, PAGE_SIZE);
        freemem((char *) newpt0, PAGE_SIZE);
        freemem((char *) newptx, PAGE_SIZE);
        freestk((char *) saddr, KERNELSTK);
        restore(mask);
        return SYSERR;
    }
    /*Lab4 2021201780:end */

    prcount++;
    prptr = &proctab[pid];

    /* Initialize process table entry for new process */
    prptr->prstate = PR_SUSP;    /* Initial state is suspended	*/
    prptr->prprio = priority;
    prptr->prstkbase = (char *) saddr;
    /*Lab3 2021201780:Begin*/
    prptr->prstkptr = prptr->uprstkptr = NULL;
    prptr->uprstkbase = (char *) usaddr;
    /*Lab3 2021201780:End*/
    //Lab4 2021201780
    prptr->prpgdir = log2ph((char *)newpgdir);
    prptr->prstklen = ssize;
    prptr->prname[PNMLEN - 1] = NULLCH;
    for (i = 0; i < PNMLEN - 1 && (prptr->prname[i] = name[i]) != NULLCH; i++);
    prptr->prsem = -1;
    prptr->prparent = (pid32) getpid();
    prptr->prhasmsg = FALSE;

    /* Set up stdin, stdout, and stderr descriptors for the shell	*/
    prptr->prdesc[0] = CONSOLE;
    prptr->prdesc[1] = CONSOLE;
    prptr->prdesc[2] = CONSOLE;

    /*Lab3 2021201780:Begin*/
    *usaddr = STACKMAGIC;
    a = (uint32 * )(&nargs + 1);
    a += nargs - 1;
    for (i = nargs ; i > 0 ; i--)
        *--usaddr = *a--;
    *--usaddr = (long) u2021201780_ret_u2k;
    usaddr = (uint32 *)(USTKBASE - ((uint32)prptr->uprstkbase - (uint32)usaddr));
    /*Lab3 2021201780:End*/

    /* Initialize kernel stack as if the process was called		*/
    *saddr = STACKMAGIC;
    savsp = (KSTKBASE - ((uint32)prptr->prstkbase - (uint32)(saddr)));
    /* Push arguments */
    a = (uint32 * )(&nargs + 1);    /* Start of args		*/
    a += nargs - 1;            /* Last argument		*/
    for (; nargs > 0; nargs--)    /* Machine dependent; copy args	*/
        *--saddr = *a--;    /* onto created process's stack	*/
    *--saddr = (long) INITRET;    /* Push on return address	*/

    prptr->prkstp = (char *)(KSTKBASE - ((uint32)prptr->prstkbase - (uint32)(saddr)));
    /*Lab3 2021201780:Begin*/
    *--saddr = BASE_USER_SS;
    *--saddr = (uint32) usaddr;
    *--saddr = 0x00000200;
    *--saddr = BASE_USER_CS;
    *--saddr = (long) funcaddr;
    *--saddr = BASE_USER_DS;
    /*Lab3 2021201780:End*/

    /* The following entries on the stack must match what ctxsw	*/
    /*   expects a saved process state to contain: ret address,	*/
    /*   ebp, interrupt mask, flags, registers, and an old SP	*/

    *--saddr = (long) k2021201780_ret_k2u;
    *--saddr = savsp;        /* This will be register ebp	*/
    /*   for process exit		*/
    // Lab4 2021201780
    savsp = (KSTKBASE - ((uint32)prptr->prstkbase - (uint32)(saddr)));        /* Start of frame for ctxsw	*/
    *--saddr = 0x00000200;        /* New process runs with	*/
    /*   interrupts enabled		*/

    /* Basically, the following emulates an x86 "pushal" instruction*/

    *--saddr = 0;            /* %eax */
    *--saddr = 0;            /* %ecx */
    *--saddr = 0;            /* %edx */
    *--saddr = 0;            /* %ebx */
    *--saddr = 0;            /* %esp; value filled in below	*/
    pushsp = saddr;            /* Remember this location	*/
    *--saddr = savsp;        /* %ebp (while finishing ctxsw)	*/
    *--saddr = 0;            /* %esi */
    *--saddr = 0;            /* %edi */

    /*Lab4 2021201780: begin */
    prptr->prstkptr = (char *)(KSTKBASE - ((uint32)prptr->prstkbase - (uint32)(saddr)));
	*pushsp = (uint32)prptr->prstkptr;

	fill_pgentry((char *)newpgdir, 0, 0, TRUE);
	invlpg((void *)newpgdir);
	fill_pgentry((char *)newpt0, 0, 0, TRUE);
	invlpg((void *)newpt0);
	fill_pgentry((char *)newptx, 0, 0, TRUE);
	invlpg((void *)newptx);

	for (i = KERNELSTK - PAGE_SIZE; i >= 0; i -= PAGE_SIZE) {
		fill_pgentry(prptr->prstkbase - i, 0, 0, TRUE);
		invlpg((void *)(prptr->prstkbase - i));
	}

    for (i = ssize - PAGE_SIZE; i >= 0; i -= PAGE_SIZE) {
        fill_pgentry(prptr->uprstkbase - i, 0, 0, TRUE);
        invlpg((void *) (prptr->uprstkbase - i));
    }

    prptr->prstkbase = (char *) KSTKBASE;
    prptr->uprstkbase = (char *) USTKBASE;
    /*Lab4 2021201780: end */
    restore(mask);
    return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local pid32

newpid(void) {
    uint32 i;            /* Iterate through all processes*/
    static pid32 nextpid = 1;    /* Position in table to try or	*/
    /*   one beyond end of table	*/

    /* Check all NPROC slots */

    for (i = 0; i < NPROC; i++) {
        nextpid %= NPROC;    /* Wrap around to beginning */
        if (proctab[nextpid].prstate == PR_FREE) {
            return nextpid++;
        } else {
            nextpid++;
        }
    }
    kprintf("newpid error\n");
    return (pid32)
    SYSERR;
}
