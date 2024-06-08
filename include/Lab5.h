#include <kbd.h>

struct	kbdcblk {
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

extern	struct	kbdcblk	kbdcb;

extern	uint8	normalmap[256];
extern	uint8	shiftcode[256];
extern	uint8	togglecode[256];
extern	uint8	shiftmap[256];
extern	uint8	ctlmap[256];

/* in file kbdcontrol.c */
extern	void	vga_init(void);
extern  devcall vga_putc(char ch, bool8);
extern	devcall	vga_erase(bool8);

/* in file kbd_dispatch.S */
extern	interrupt	kbd_dispatch(void);

/* in file kbd_read.c */
extern	devcall	kbd_read(struct dentry *, char *, int32);

/* in file kbd_getc.c */
extern	devcall	kbd_getc(struct dentry *);

/* in file kbd_putc.c */
extern	devcall	kbd_putc(struct dentry *, char);

/* in file kbd_handler.c */
extern	void	kbd_handler(void);

/* in file kbd_init.c */
extern	devcall	kbd_init(void);

/* in file kbdcontrol.c */
extern void vga_cursor_left(void);
extern void vga_cursor_right(void);