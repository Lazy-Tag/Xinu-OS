/* xsh_devdump.c  - xsh_devdump */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_devdump - shell command to print info from the device switch table
 *------------------------------------------------------------------------
 */
shellcmd xsh_devdump (
	 int	nargs,			/* number of arguments		*/
	 char	*args[]			/* list of arguments		*/
 	)
{
	struct	dentry	*devptr;	/* pointer to device entry	*/
	int32	i;			/* walks through device table	*/

	/* No arguments are expected */

	if (nargs > 1) {
		u2021201780_fprintf(stderr, "No arguments allowed\n", 0);
		return 1;
	}

	/* Walk through device table */

	u2021201780_printf("Device     Name     Minor\n", 0);
	u2021201780_printf("------ ------------ -----\n", 0);

	for (i = 0; i < NDEVS; i++) {
		devptr = &devtab[i];
		u2021201780_printf("%4d   %-12s %3d\n", 3, i, devptr->dvname,
			devptr->dvminor);
	}
	return 0;
}
