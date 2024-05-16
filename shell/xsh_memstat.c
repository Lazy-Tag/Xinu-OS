/* xsh_memstat.c - xsh_memstat */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

static	void	printMemUse(void);
static	void	printFreeList(void);

/*------------------------------------------------------------------------
 * xsh_memstat - Print statistics about memory use and dump the free list
 *------------------------------------------------------------------------
 */
shellcmd xsh_memstat(int nargs, char *args[])
{

	/* For argument '--help', emit help about the 'memstat' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		u2021201780_printf("use: %s \n\n", 1, args[0]);
		u2021201780_printf("Description:\n", 0);
		u2021201780_printf("\tDisplays the current memory use and prints the\n", 0);
		u2021201780_printf("\tfree list.\n", 0);
		u2021201780_printf("Options:\n", 0);
		u2021201780_printf("\t--help\t\tdisplay this help and exit\n", 0);
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		u2021201780_fprintf(stderr, "%s: too many arguments\n", 1, args[0]);
		u2021201780_fprintf(stderr, "Try '%s --help' for more information\n",
				1, args[0]);
		return 1;
	}

	printMemUse();
	printFreeList();

	return 0;
}


/*------------------------------------------------------------------------
 * printFreeList - Walk the list of free memory blocks and print the
 *			location and size of each
 *------------------------------------------------------------------------
 */
static void printFreeList(void)
{
	struct memblk *block;

	/* Output a heading for the free list */

	u2021201780_printf("Free List:\n", 0);
	u2021201780_printf("Block address  Length (dec)  Length (hex)\n", 0);
	u2021201780_printf("-------------  ------------  ------------\n", 0);
	
	for (block = memlist.mnext; block != NULL; block = block->mnext) {
		u2021201780_printf("  0x%08x    %9d     0x%08x\n", 3, block,
			block->mlength, block->mlength);
	}
	u2021201780_printf("\n", 0);
}

extern void start(void);
extern void *_end;

/*------------------------------------------------------------------------
 * printMemUse - Print statistics about memory use
 *------------------------------------------------------------------------
 */
static void printMemUse(void)
{
	int i;				/* Index into process table	*/
	uint32 code = 0;		/* Total Xinu code memory	*/
	uint32 stack = 0;		/* Total used stack memory	*/
	uint32 kheap = 0;		/* Free kernel heap memory	*/
	uint32 kfree = 0;		/* Total free memory		*/
	struct memblk *block;	 	/* Ptr to memory block		*/

	/* Calculate amount of text memory */

	code = (uint32)&_end - (uint32)start;

	/* Calculate amount of allocated stack memory */
	/*  Skip the NULL process since it has a private stack */

	for (i = 1; i < NPROC; i++) {
		if (proctab[i].prstate != PR_FREE) {
			stack += (uint32)proctab[i].prstklen;
		}
	}

	/* Calculate the amount of memory on the free list */

	for (block = memlist.mnext; block != NULL; block = block->mnext) {
		kfree += block->mlength;
	}

	/* Calculate the amount of free kernel heap memory */

	kheap = kfree - stack;

	/* Output statistics on current memory use */

	u2021201780_printf("Current system memory statistics:\n", 0);
	u2021201780_printf("---------------------------------\n", 0);
	u2021201780_printf("%10d bytes (0x%08x) of Xinu code\n", 2, code, code);
	u2021201780_printf("%10d bytes (0x%08x) of allocated stack space\n", 2, stack, stack);
	u2021201780_printf("%10d bytes (0x%08x) of available kernel heap space\n\n", 2, kheap, kheap);
}
