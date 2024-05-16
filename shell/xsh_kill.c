/* xsh_kill.c - xsh_kill */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_kill - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_kill(int nargs, char *args[]) {

	int32	retval;			/* return value			*/
	pid32	pid;			/* ID of process to kill	*/
	char	ch;			/* next character of argument	*/
	char	*chptr;			/* walks along argument string	*/

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		u2021201780_printf("Usage: %s PID\n\n", 1, args[0]);
		u2021201780_printf("Description:\n", 0);
		u2021201780_printf("\tterminates a process\n", 0);
		u2021201780_printf("Options:\n", 0);
		u2021201780_printf("\tPID \tthe ID of a process to terminate\n", 0);
		u2021201780_printf("\t--help\tdisplay this help and exit\n", 0);
		return OK;
	}

	/* Check argument count */

	if (nargs != 2) {
		u2021201780_fprintf(stderr, "%s: incorrect argument\n", 1, args[0]);
		u2021201780_fprintf(stderr, "Try '%s --help' for more information\n",
			1, args[0]);
		return SYSERR;
	}

	/* compute process ID from argument string */

	chptr = args[1];
	ch = *chptr++;
	pid = 0;
	while(ch != NULLCH) {
		if ( (ch < '0') || (ch > '9') ) {
			u2021201780_fprintf(stderr, "%s: non-digit in process ID\n",
				1, args[0]);
			return 1;
		}
		pid = 10*pid + (ch - '0');
		ch = *chptr++;
	}
	if (pid == 0) {
		u2021201780_fprintf(stderr, "%s: cannot kill the null process\n",
			1, args[0]);
		return 1;
	}

	retval = kill(pid);
	if (retval == SYSERR) {
		u2021201780_fprintf(stderr, "%s: cannot kill process %d\n",
			1, args[0], pid);
		return 1;
	}
	return 0;
}
