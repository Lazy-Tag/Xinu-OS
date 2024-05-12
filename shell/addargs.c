/* addargs.c - addargs */

#include <xinu.h>

/*Lab4 2021201780:Begin*/
status	upload_args(
	  uint32	*argloc,	/* Lowest location in the	*/
	  				/*   process stack where the	*/
					/*   args array will be stored	*/
	  uint32	*stk_end,
	  char		*stk_base,
	  int32		ntok,		/* Count of arguments		*/
	  int32		tok[],		/* Index of tokens in tokbuf	*/
	  int32		tlen,		/* Length of data in tokbuf	*/
	  char		*tokbuf,	/* Array of null-term. tokens	*/
	  void 		*dummy		/* Dummy argument that was	*/
					/*   used at creation and must	*/
					/*   be replaced by a pointer	*/
					/*   to an argument vector	*/
	)
{
	char	*argstr;		/* Location in process's stack	*/
					/*   to place arg strings	*/
	uint32	*real_loc;
	uint32	*search;		/* pointer that searches for	*/
					/*   dummy argument on stack	*/
	uint32	*aptr;			/* Walks through args array	*/
	int32	i;			/* Index into tok array		*/

	/* Compute the first location beyond args array for the strings	*/

	real_loc = (uint32 *)(stk_base + 4 - ((uint32)stk_end - (uint32)argloc));

	argstr = (char *) (real_loc + (ntok+1)); /* +1 for a null ptr	*/

	/* Set each location in the args vector to be the address of	*/
	/*	string area plus the offset of this argument		*/

	for (aptr=argloc, i=0; i < ntok; i++) {
		*aptr++ = (uint32) (argstr + tok[i]);
	}

	/* Add a null pointer to the args array */

	*aptr++ = (uint32)NULL;

	/* Copy the argument strings from tokbuf into process's	stack	*/
	/*	just beyond the args vector				*/

	memcpy(aptr, tokbuf, tlen);

	/* Find the second argument in process's stack */

	for (search = (uint32 *)argloc; search < (uint32 *)stk_end; search++) {
		/* If found, replace with the address of the args vector*/
		if (*search == (uint32)dummy) {
			*search = (uint32)real_loc;
			return OK;
		}
	}

	return SYSERR;
}
/*Lab4 2021201780:End*/

/*------------------------------------------------------------------------
 *  addargs  -  Add local copy of argv-style arguments to the stack of
 *		  a command process that has been created by the shell
 *------------------------------------------------------------------------
 */
status	addargs(
	  pid32		pid,		/* ID of process to use		*/
	  int32		ntok,		/* Count of arguments		*/
	  int32		tok[],		/* Index of tokens in tokbuf	*/
	  int32		tlen,		/* Length of data in tokbuf	*/
	  char		*tokbuf,	/* Array of null-term. tokens	*/
	  void 		*dummy		/* Dummy argument that was	*/
					/*   used at creation and must	*/
					/*   be replaced by a pointer	*/
					/*   to an argument vector	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();
	bool8 error = FALSE;

	/* Check argument count and data length */

	if ( (ntok <= 0) || (tlen < 0) ) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	/*Lab4 2020200671:Begin*/
	/* Map stack of the new process to heap */
    pgtab *pgdir = (pgtab *)getheap(sizeof(pgtab));
	if (pgdir == (pgtab *)SYSERR) {
		restore(mask);
		return SYSERR;
	}
	fill_pgentry((char *)pgdir, prptr->prpgdir, PT_ENTRY_P | PT_ENTRY_W, FALSE);

	pgtab *pgtable = (pgtab *)getheap(sizeof(pgtab));
	if (pgtable == (pgtab *)SYSERR) {
		fill_pgentry((char *)pgdir, 0, 0, TRUE);
		invlpg((void *)pgdir);
		restore(mask);
		return SYSERR;
	}
	fill_pgentry((char *)pgtable, getaddr(pgdir->entry[0]), PT_ENTRY_P | PT_ENTRY_W, FALSE);

	char* kstk = getheap(KERNELSTK);
	if (kstk == (char *)SYSERR) {
		fill_pgentry((char *)pgdir, 0, 0, TRUE);
		invlpg((void *)pgdir);
		fill_pgentry((char *)pgtable, 0, 0, TRUE);
		invlpg((void *)pgtable);
		restore(mask);
		return SYSERR;
	}
	fill_pgentry(kstk, getaddr(pgtable->entry[1 + (uint32)&end / PAGE_SIZE]), PT_ENTRY_P | PT_ENTRY_W, FALSE);

	fill_pgentry((char *)pgtable, 0, 0, FALSE);
	invlpg((void *)pgtable);

	fill_pgentry((char *)pgtable, getaddr(pgdir->entry[1015]), PT_ENTRY_P | PT_ENTRY_W, FALSE);
	char* ustk = getheap(prptr->prstklen);
	if (ustk == (char *)SYSERR) {
		fill_pgentry((char *)pgdir, 0, 0, TRUE);
		invlpg((void *)pgdir);
		fill_pgentry((char *)pgtable, 0, 0, TRUE);
		invlpg((void *)pgtable);
		fill_pgentry(kstk, 0, 0, TRUE);
		invlpg((void *)kstk);
		restore(mask);
		return SYSERR;
	}
	fill_pgentry(ustk, getaddr(pgtable->entry[1023]), PT_ENTRY_P | PT_ENTRY_W, FALSE);
	fill_pgentry((char *)pgtable, 0, 0, FALSE);
	invlpg((void *)pgtable);

	if (upload_args((uint32 *)ustk, (uint32 *)(ustk + prptr->prstklen),
		    prptr->uprstkbase, ntok, tok, tlen, tokbuf, dummy) == SYSERR) {
			error = TRUE;
	}

	if (upload_args((uint32 *)kstk, (uint32 *)(kstk + KERNELSTK),
		    prptr->prstkbase, ntok, tok, tlen, tokbuf, dummy) == SYSERR) {
			error = TRUE;
	}

	fill_pgentry(kstk, 0, 0, FALSE);
	invlpg((void *)kstk);
	fill_pgentry(ustk, 0, 0, FALSE);
	invlpg((void *)ustk);
	fill_pgentry((char *)pgdir, 0, 0, FALSE);
	invlpg((void *)pgdir);

	restore(mask);
	return error ? SYSERR : OK;
	/*Lab4 2020200671:End*/
}
