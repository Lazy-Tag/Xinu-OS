/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_echo - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_echo(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/

	if (nargs > 1) {
        u2021201780_printf("%s", 1, args[1]);

		for (i = 2; i < nargs; i++) {
			u2021201780_printf(" %s", 1, args[i]);
		}
	}
    u2021201780_printf("\n", 0);

	return 0;
}
