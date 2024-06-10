#include <kbd.h>

struct	vgacblk {
	char	*tyihead;		/* Next input char to read	*/
	char	*tyitail;		/* Next slot for arriving char	*/
	char	tyibuff[TY_IBUFLEN];	/* Input buffer			*/
	sid32	tyisem;			/* Input semaphore		*/
	int32	tyicursor;		/* Current cursor position	*/
};

#define	KBD_WIDTH	80
#define	KBD_HEIGHT	25
#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5
#define BLUE_BAN "\033[34m\033[1m"
#define ORIGIN_BAN "\033[0m\n"

static	uint16	(*disp)[KBD_HEIGHT][KBD_WIDTH] = 0xB8000;
static	uint16	color = 0x0700;

extern	struct	vgacblk	kbdcb;

extern	uchar	normalmap[256];
extern	uchar	shiftcode[256];
extern	uchar	togglecode[256];
extern	uchar	shiftmap[256];
extern	uchar	ctlmap[256];

/* in file kbdcontrol.c */
extern  devcall vga_putc(char ch, bool8);
extern	devcall	vga_erase(bool8);

/* in file vgadispatch.S */
extern	interrupt	vgadispatch(void);

/* in file vgaread.c */
extern	devcall	vgaread(struct dentry *, char *, int32);

/* in file vgawrite.c */
extern	devcall	vgawrite(struct dentry *, char *, int32);

/* in file vgagetc.c */
extern	devcall	vgagetc(struct dentry *);

/* in file vgaputc.c */
extern	devcall	vgaputc(struct dentry *, char);

/* in file vgahandler.c */
extern	void	vgahandler(void);

/* in file vgainit.c */
extern	devcall	vgainit(void);

/* in file vgacontrol.c */
extern void vga_cursor_left(void);
extern void vga_cursor_right(void);

/* in file multiinit.c */
extern  devcall multiinit(struct dentry *);

/* in file multiputc.c */
extern  devcall multiputc(struct dentry *, char);

/* in file multiwrite.c */
extern  devcall multiwrite(struct dentry *, char*, int32);

/* in file multidispatch.c */
extern  interrupt multidispatch(void);

/* in file ttyhandle_in.c */
extern void	erase1(struct ttycblk *, struct uart_csreg *);
extern void	echoch(char, struct ttycblk *, struct uart_csreg *);
extern void	eputc(char, struct ttycblk *, struct uart_csreg *);