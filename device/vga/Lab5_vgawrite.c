/* vgawrite.c - vgawrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  vgawrite  -  Write character(s) to a vga device (interrupts disabled)
 *------------------------------------------------------------------------
 */
devcall	vgawrite(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char	*buff,			/* Buffer of characters		*/
	  int32	count 			/* Count of character to write	*/
	)
{
	/* Handle negative and zero counts */

	if (count < 0) {
		return SYSERR;
	} else if (count == 0){
		return OK;
	}

	/* Write count characters one at a time */

	for (; count>0 ; count--) {
		vgaputc(devptr, *buff++);
	}
	return OK;
}
