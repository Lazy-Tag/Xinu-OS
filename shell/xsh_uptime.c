/* xsh_uptime.c - xsh_uptime */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------
 * xsh_uptime - shell to print the time the system has been up
 *------------------------------------------------------------------------
 */
shellcmd xsh_uptime(int nargs, char *args[])
{
	uint32	days, hrs, mins, secs;	/* days, hours, minutes,  and	*/
					/*  seconds since system boot	*/
	uint32	secperday = 86400;	/* seconds in a day		*/
	uint32	secperhr  =  3600;	/* seconds in an hour		*/
	uint32	secpermin =    60;	/* seconds in a minute		*/	

	/* For argument '--help', emit help about the 'uptime' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		u2021201780_printf("Use: %s\n\n", 1, args[0]);
		u2021201780_printf("Description:\n", 0);
		u2021201780_printf("\tDisplays time since the system booted\n", 0);
		u2021201780_printf("Options:\n", 0);
		u2021201780_printf("\t--help\t display this help and exit\n", 0);
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		u2021201780_fprintf(stderr, 1, "%s: too many arguments\n", args[0]);
		u2021201780_fprintf(stderr, 1, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	secs = clktime;		/* total seconds since boot */

	/* subtract number of whole days */

	days  = secs/secperday;
	secs -= days*secperday;

	/* subtract number of hours */

	hrs   = secs/secperhr;
	secs -= hrs*secperhr;

	/* subtract number of minutes */

	mins  = secs/secpermin;
	secs -= mins*secpermin;

	u2021201780_printf("Xinu has been up ", 0);
	if (days > 0) {
		u2021201780_printf(" %d day(s) ", 1, days);
	}

	if (hrs > 0) {
		u2021201780_printf(" %d hour(s) ", 1, hrs);
	}

	if (mins > 0) {
		u2021201780_printf(" %d minute(s) ", 1, mins);
	}

	if (secs > 0) {
		u2021201780_printf(" %d second(s) ", 1, secs);
	}
	u2021201780_printf("\n", 0);

	return 0;
}
