/* vgacursor.c - cursor operation*/
#include <xinu.h>

void set_cursor(uint16 pos) {
	outb(VGA_INDEX_PORT, 14);
	outb(VGA_DATA_PORT, pos >> 8);
	outb(VGA_INDEX_PORT, 15);
	outb(VGA_DATA_PORT, pos);
}

uint16 get_cursor(void) {
	uint16		pos;

	outb(VGA_INDEX_PORT, 14);
	pos = inb(VGA_DATA_PORT) << 8;
	outb(VGA_INDEX_PORT, 15);
	pos |= inb(VGA_DATA_PORT);
	return pos;
}

void vga_cursor_left(void) {
    uint16 pos = get_cursor();
    if (pos > 0) {
        set_cursor(pos - 1);
    }
}

void vga_cursor_right(void) {
    uint16 pos = get_cursor();
    if (pos < KBD_WIDTH * KBD_HEIGHT - 1) {
        set_cursor(pos + 1);
    }
}
