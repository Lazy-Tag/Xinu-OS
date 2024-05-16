/* xsh_sleep.c - xsh_sleep */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_sleep  -  Shell command to delay for a specified number of seconds
 *------------------------------------------------------------------------
 */
shellcmd xsh_sleep(int nargs, char *args[])
{
	int32	delay;			/* Delay in seconds		*/
	char	*chptr;			/* Walks through argument	*/
	char	ch;			/* Next character of argument	*/

	/* For argument '--help', emit help about the 'sleep' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		u2021201780_printf("Use: %s\n\n", 1, args[0]);
		u2021201780_printf("Description:\n", 0);
		u2021201780_printf("\tDelay for a specified number of seconds\n", 0);
		u2021201780_printf("Options:\n", 0);
		u2021201780_printf("\t--help\t display this help and exit\n", 0);
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 2) {
		u2021201780_fprintf(stderr, 1, "%s: too many arguments\n", args[0]);
		u2021201780_fprintf(stderr, 1, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	if (nargs != 2) {
		u2021201780_fprintf(stderr, 1, "%s: argument in error\n", args[0]);
		u2021201780_fprintf(stderr, 1, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	chptr = args[1];
	ch = *chptr++;
	delay = 0;
	while (ch != NULLCH) {
		if ( (ch < '0') || (ch > '9') ) {
			u2021201780_fprintf(stderr, "%s: nondigit in argument\n",
				1, args[0]);
			return 1;
		}
		delay = 10*delay + (ch - '0');
		ch = *chptr++;
	}
	u2021201780_sleep(delay);
	return 0;
}
