/* multiwrite.c - multiwrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  multiwrite  -  Write character(s) to a tty device and a VGA device
 *------------------------------------------------------------------------
 */
devcall	multiwrite(
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
		multiputc(devptr, *buff++);
	}
	return OK;
}
