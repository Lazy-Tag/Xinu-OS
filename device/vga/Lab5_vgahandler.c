/* Lab5_vgahandler.c - vgahandler */

#include <xinu.h>

void vgahandler(void) {
    static uint32 shift;
    static uint8 *charcode[] = {normalmap, shiftmap, ctlmap, ctlmap};

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
        signaln(kbdcb.tyisem, icursor + 1);
        return;
    } else if (c == TY_TAB) {
        vga_putc(TY_UPARROW, FALSE);
        vga_putc('I', FALSE);
        *kbdcb.tyitail++ = '\t';
        kbdcb.tyicursor++;
        if (kbdcb.tyitail >= &kbdcb.tyibuff[TY_IBUFLEN]) {
            kbdcb.tyitail = kbdcb.tyibuff;
        }
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

    *kbdcb.tyitail++ = c;
    kbdcb.tyicursor++;
    if (kbdcb.tyitail >= &kbdcb.tyibuff[TY_IBUFLEN]) {
        kbdcb.tyitail = kbdcb.tyibuff;
    }
}