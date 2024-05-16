/* memory.h - roundmb, truncmb, freestk */

#define	PAGE_SIZE	4096

/*----------------------------------------------------------------------
 * roundmb, truncmb - Round or truncate address to memory block size
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  Free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define	freestk(p,len)	freemem((char *)((uint32)(p)		\
				- ((uint32)roundmb(len))	\
				+ (uint32)sizeof(uint32)),	\
				(uint32)roundmb(len) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	};
extern	struct	memblk	memlist;	/* Head of free memory list	*/
extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/


/* Added by linker */

extern	int	text;			/* Start of text segment	*/
extern	int	etext;			/* End of text segment		*/
extern	int	data;			/* Start of data segment	*/
extern	int	edata;			/* End of data segment		*/
extern	int	bss;			/* Start of bss segment		*/
extern	int	ebss;			/* End of bss segment		*/
extern	int	end;			/* End of program		*/

/*Lab4 2021201780: Begin*/
#define roundpg(x) (uint32)((PAGE_SIZE - 1 + (uint32)(x)) & (~(PAGE_SIZE - 1)))
#define truncpg(x)	 (uint32)(((uint32)(x)) & (~(PAGE_SIZE - 1)))

#define ph2recpg(x)		(((uint32)(x) / PAGE_SIZE) * 4 + 0x400000)
#define rec2phpg(x)		(((uint32)(x) - 0x400000) / 4 * PAGE_SIZE)

#define KSTKBASE		((uint32)&end + 2 * PAGE_SIZE - sizeof(uint32))
#define USTKBASE		((uint32)maxheap - sizeof(uint32))
#define PT_ENTRY_P		1
#define PT_ENTRY_W		2
#define PT_ENTRY_U		4
#define getaddr(x)		((x) & 0xFFFFF000)

#define PT_NENTRY		1024
#define USERPG 1015
typedef struct {
	uint32 entry[PT_NENTRY];
} pgtab;
extern uint32 *freelist;
extern pgtab *pgdir;

/*Lab4 2021201780: End*/
