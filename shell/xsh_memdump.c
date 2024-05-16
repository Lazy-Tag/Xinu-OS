/* xsh_memdump.c - xsh_memdump */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

static	uint32	parseval(char *);
extern	uint32	start;

/*------------------------------------------------------------------------
 * xsh_memdump - dump a region of memory by displaying values in hex
 *			 and ascii
 *------------------------------------------------------------------------
 */
shellcmd xsh_memdump(int nargs, char *args[])
{
	bool8	force = FALSE;		/* ignore address sanity checks	*/
	uint32	begin;			/* begining address		*/
	uint32	stop;			/* last address to dump		*/
	uint32	length;			/* length of region to dump	*/
	int32	arg;			/* index into args array	*/
	uint32	l;			/* counts length during dump	*/
	int32	i;			/* counts words during dump	*/
	uint32	*addr;			/* address to dump		*/
	char	*chptr;			/* character address to dump	*/
	char	ch;			/* next character to print	*/

	/* For argument '--help', emit help about the 'memdump' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		u2021201780_printf("Use: %s [-f] Address Length\n\n", 1, args[0]);
		u2021201780_printf("Description:\n", 0);
		u2021201780_printf("\tDumps Length bytes of memory begining at the\n", 0);
		u2021201780_printf("\tspecified starting address (both the address\n", 0);
		u2021201780_printf("\tand length can be specified in decimal or hex)\n", 0);
		u2021201780_printf("Options:\n", 0);
		u2021201780_printf("\t-f         ignore sanity checks for addresses\n", 0);
		u2021201780_printf("\tAddress    memory address at which to start\n", 0);
		u2021201780_printf("\tLength     the number of bytes to dump\n", 0);
		u2021201780_printf("\t--help     display this help and exit\n", 0);
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs < 3 || nargs > 4) {
		u2021201780_fprintf(stderr, "%s: incorrect number of arguments\n",
				1, args[0]);
		u2021201780_fprintf(stderr, "Try '%s --help' for more information\n",
				1, args[0]);
		return 1;
	}

	arg = 1;
	if (strncmp(args[arg], "-f", 2) == 0) {
		force = TRUE;
		arg++;
		nargs --;
	}

	if (nargs != 3) {
		u2021201780_fprintf(stderr, "%s: too few arguments\n", 1, args[0]);
		u2021201780_fprintf(stderr, "Try '%s --help' for more information\n",
				1, args[0]);
		return 1;
	}

	if ( (begin=parseval(args[arg])) == 0 ) {
		u2021201780_fprintf(stderr, "%s: invalid begining address\n",
				1, args[0]);
		return 1;
	}
	if ( (length =parseval(args[arg+1])) == 0 ) {
		u2021201780_fprintf(stderr, "%s: invalid length address\n",
				1, args[0]);
		return 1;
	}

	/* Round begining address down to multiple of four and round	*/
	/*	length up to a multiple of four				*/

	begin &= ~0x3;
	length = (length + 3) & ~0x3;

	/* Add length to begin address */

	stop = begin + length;

	/* verify that the address and length are reasonable */

	if ( force || ( (begin >= (uint32)&start) && (stop > begin) &&
					(((void *)stop) < maxheap)) ) {

		/* values are valid; perform dump */

		chptr = (char *)begin;
		for (l=0; l<length; l+=16) {
			u2021201780_printf("%08x: ", 1, begin);
			addr = (uint32 *)begin;
			for (i=0; i<4; i++) {
				u2021201780_printf("%08x ", 1, *addr++);
			}
			u2021201780_printf("  *", 0);
			for (i=0; i<16; i++) {
				ch = *chptr++;
				if ( (ch >= 0x20) && (ch <= 0x7e) ) {
					u2021201780_printf("%c", 1, ch);
				} else {
					u2021201780_printf(".", 0);
				}
			}
			u2021201780_printf("*\n", 0);
			begin += 16;
		}
		return 0;
	} else {
		u2021201780_printf("Values are out of range; use -f to force\n", 0);
	}
	return 1;
}

/*------------------------------------------------------------------------
 * parse - parse an argument that is either a decimal or hex value
 *------------------------------------------------------------------------
 */
static	uint32	parseval(
	  char	*string			/* argument string to parse	*/
	)
{
	uint32	value;			/* value to return		*/
	char	ch;			/* next character		*/
	

	value = 0;

	/* argument string must consists of decimal digits or	*/
	/*	0x followed by hex digits			*/

	ch = *string++;
	if (ch == '0') {		/* hexadecimal */
		if (*string++ != 'x') {
			return 0;
		}
		for (ch = *string++; ch != NULLCH; ch = *string++) {
			if ((ch >= '0') && (ch <= '9') ) {
				value = 16*value + (ch - '0');
			} else if ((ch >= 'a') && (ch <= 'f') ) {
				value = 16*value + 10 + (ch - 'a');
			} else if ((ch >= 'A') && (ch <= 'F') ) {
				value = 16*value + 10 + (ch - 'A');
			} else {
				return 0;
			}
		}
	} else {			/* decimal */
		while (ch != NULLCH) {
			if ( (ch < '0') || (ch > '9') ) {
				return 0;
			}
			value = 10*value + (ch - '0');
			ch = *string++;
		}
	}
	return value;
}
