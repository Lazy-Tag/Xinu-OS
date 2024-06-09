/* multihandler.c - multihandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  multihandler  -  Handle an interrupt for a multi device
 *------------------------------------------------------------------------
 */
void multihandler(void) {
	static uint32 shift;
    static uint8 *charcode[] = {normalmap, shiftmap, ctlmap, ctlmap};

    struct	dentry	*devptr;	/* Address of device control blk*/
	struct	ttycblk	*typtr;		/* Pointer to ttytab entry	*/
	struct	uart_csreg *csrptr;	/* Address of UART's CSR	*/

    devptr = (struct dentry *) &devtab[CONSOLE];
	csrptr = (struct uart_csreg *) devptr->dvcsr;
    typtr = &ttytab[ devptr->dvminor ];

    uint32 st, data, c;
    int32 avail;

    st = inb(KBSTATP);
    if ((st & KBS_DIB) == 0) {
        return;
    }
    data = inb(KBDATAP);

    if (data == 0xE0) {
        shift |= E0ESC;
        return;
    } else if (data & 0x80) {
        data = (shift & E0ESC) ? data : (data & 0x7F);
        shift &= ~(shiftcode[data] | E0ESC);
        return;
    } else if (shift & E0ESC) {
        switch (data) {
            case TY_LEFT:
                if (kbdcb.tyicursor > 0) {
                    kbdcb.tyicursor--;
                    kbdcb.tyitail--;
                    if (kbdcb.tyitail < kbdcb.tyibuff) {
                        kbdcb.tyitail = &kbdcb.tyibuff[TY_IBUFLEN - 1];
                    }
                    vga_cursor_left();
                }
                break;
            case TY_RIGHT:
                if (kbdcb.tyitail < &kbdcb.tyibuff[TY_IBUFLEN - 1] && *kbdcb.tyitail != '\0') {
                    kbdcb.tyicursor++;
                    kbdcb.tyitail++;
                    if (kbdcb.tyitail >= &kbdcb.tyibuff[TY_IBUFLEN]) {
                        kbdcb.tyitail = kbdcb.tyibuff;
                    }
                    vga_cursor_right();
                }
                break;
        }
        shift &= ~E0ESC;
        return;
    }

    shift |= shiftcode[data];
    shift ^= togglecode[data];

    c = charcode[shift & (CTL | SHIFT)][data];
    if (!c) return;

    if ((c == TY_RETURN) && typtr->tyicrlf) {
		c = TY_NEWLINE;
	}

    typtr->tyoheld = FALSE;		/* Any other char starts output */

    if (c == TY_BACKSP || c == TY_BACKSP2) {
        if (kbdcb.tyicursor > 0) {
            kbdcb.tyicursor--;

            kbdcb.tyitail--;
            if (kbdcb.tyitail < kbdcb.tyibuff) {
                kbdcb.tyitail = &kbdcb.tyibuff[TY_IBUFLEN - 1];
            }
            c = *kbdcb.tyitail;
            vga_erase(c < TY_BLANK || c == 0177);
        }

        if (typtr->tyicursor > 0) {
            typtr->tyicursor--;
            erase1(typtr, csrptr);
        }
        return;
    } else if (c == TY_NEWLINE || c == TY_RETURN) {
        int32 icursor = kbdcb.tyicursor;
        vga_putc(TY_RETURN, FALSE);
        vga_putc(TY_NEWLINE, FALSE);
        *kbdcb.tyitail++ = c;
        if (kbdcb.tyitail >= &kbdcb.tyibuff[TY_IBUFLEN]) {
            kbdcb.tyitail = kbdcb.tyibuff;
        }
        kbdcb.tyicursor = 0;

        if (typtr->tyiecho) {
            echoch(c, typtr, csrptr);
        }
        *typtr->tyitail++ = c;
        if (typtr->tyitail>=&typtr->tyibuff[TY_IBUFLEN]) {
            typtr->tyitail = typtr->tyibuff;
        }
        /* Make entire line (plus \n or \r) available */
        signaln(kbdcb.tyisem, icursor + 1);
        signaln(typtr->tyisem, typtr->tyicursor + 1);
        typtr->tyicursor = 0; 	/* Reset for next line	*/
        return;
    } else if (c == TY_ESC) return;

    avail = semcount(kbdcb.tyisem);
    if (avail < 0) avail = 0;
    if (avail + kbdcb.tyicursor >= TY_IBUFLEN - 1) {
        return;
    }

    if (shift & CAPSLOCK) {
        if (c >= 'a' && c <= 'z') {
            c += 'A' - 'a';
        } else if (c >= 'A' && c <= 'Z') {
            c -= 'A' - 'a';
        }
    }

    if (c < TY_BLANK || c == 0177) {
        vga_putc(TY_UPARROW, FALSE);
        vga_putc(c + 0100, FALSE);
    } else {
        vga_putc(c, FALSE);
    }
    echoch(c, typtr, csrptr);

    *kbdcb.tyitail++ = c;
    kbdcb.tyicursor++;
    if (kbdcb.tyitail >= &kbdcb.tyibuff[TY_IBUFLEN]) {
        kbdcb.tyitail = kbdcb.tyibuff;
    }

    typtr->tyicursor++;
    *typtr->tyitail++ = c;

    /* Wrap around if needed */

    if (typtr->tyitail >= &typtr->tyibuff[TY_IBUFLEN]) {
        typtr->tyitail = typtr->tyibuff;
    }
}
