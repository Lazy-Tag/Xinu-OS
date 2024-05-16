/* xsh_clear.c - xsh_clear */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_clear - clear the display window (assumes xterm / VT100)
 *------------------------------------------------------------------------
 */
shellcmd xsh_clear(int nargs, char *args[])
{

    /* Insure no arguments were passed */

    if (nargs > 1) {
	u2021201780_fprintf(stderr,"use is: %s\n", 1, args[0]);
	return 1;
    }

    u2021201780_printf("\033[0m\033[2J\033[H\n", 0);
    return 0;
}
