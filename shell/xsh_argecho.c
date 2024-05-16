/* xsh_argecho.c - xsh_argecho */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_argecho - display argecho message that lists shell commands
 *------------------------------------------------------------------------
 */
shellcmd xsh_argecho(int nargs, char *args[])
{
	int32	i;

	u2021201780_printf("\n\nThe %d arguments are:\n", 1, nargs);
	for (i = 0; i < nargs; i++) {
		u2021201780_printf("    %2d: %s\n", 2, i, args[i]);
	}
	u2021201780_printf("\n", 0);

	return 0;
}
