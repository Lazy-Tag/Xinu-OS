/* diskseek.c - diskseek */

#include <xinu.h>

/*------------------------------------------------------------------------
 * diskseek  -  Seek to a specified position in a disk
 *------------------------------------------------------------------------
 */
devcall	diskseek (
	  struct dentry *devptr,	/* Entry in device switch table */
	  uint32	offset		/* Byte position in the file	*/
	)
{
	struct	diskcblk	*diskptr;

	diskptr = &disktab[devptr->dvminor];
	diskptr->lba_addr = offset;

	return (devcall)OK;
}
