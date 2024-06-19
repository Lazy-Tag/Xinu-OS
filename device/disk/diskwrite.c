/* diskwrite.c */

#include <xinu.h>

/*------------------------------------------------------------------------
 * diskwrite  -  write data to disk
 *------------------------------------------------------------------------
 */
devcall	diskwrite(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char		*buff,		/* Buffer of characters		*/
	  int32		count 		/* Count of character to read	*/
	)
{
	int32		i;
    struct	diskcblk	*diskptr;
	diskptr = &disktab[devptr->dvminor];

	for (i = 0; i < count; i++) {
		diskputc(devptr, buff[i]);
	}

    diskflush(devptr);

	return (devcall)OK;
}
