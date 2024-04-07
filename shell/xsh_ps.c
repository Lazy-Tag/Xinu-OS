/* xsh_ps.c - xsh_ps */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_ps - shell command to print the process table
 *------------------------------------------------------------------------
 */
shellcmd xsh_ps(int nargs, char *args[])
{
	struct	procent	*prptr;		/* pointer to process		*/
	int32	i;			/* index into proctabl		*/
	char *pstate[]	= {		/* names for process states	*/
		"free ", "curr ", "ready", "recv ", "sleep", "susp ",
		"wait ", "rtime"};

	/* For argument '--help', emit help about the 'ps' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		u2021201780_printf("Use: %s\n\n", 1, args[0]);
		u2021201780_printf("Description:\n", 0);
		u2021201780_printf("\tDisplays information about running processes\n", 0);
		u2021201780_printf("Options:\n", 0);
		u2021201780_printf("\t--help\t display this help and exit\n", 0);
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		u2021201780_printf("%s: too many arguments\n", 1, args[0]);
		u2021201780_printf("Try '%s --help' for more information\n", 1,
				args[0]);
		return 1;
	}

	/* Print header for items from the process table */

	u2021201780_printf("%3s %-16s %5s %4s %4s %10s %-10s %10s\n", 8,
		   "Pid", "Name", "State", "Prio", "Ppid", "Stack Base",
		   "Stack Ptr", "Stack Size");

	u2021201780_printf("%3s %-16s %5s %4s %4s %10s %-10s %10s\n", 8,
		   "---", "----------------", "-----", "----", "----",
		   "----------", "----------", "----------");

	/* Output information for each process */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		if (prptr->prstate == PR_FREE) {  /* skip unused slots	*/
			continue;
		}
		u2021201780_printf("%3d %-16s %s %4d %4d 0x%08X 0x%08X %8d\n", 8,
			i, prptr->prname, pstate[(int)prptr->prstate],
			prptr->prprio, prptr->prparent, prptr->prstkbase,
			prptr->prstkptr, prptr->prstklen);
	}
	return 0;
}
