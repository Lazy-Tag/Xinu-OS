/* vgacontrol.c - vga operation */

#include <xinu.h>

bool8 vga_shellbanner(char);

devcall vga_putc(char ch, bool8 force) {
    uint16 cursor_pos, row, col;
    uint16 i;
    static bool8 shell_banner = FALSE;

    cursor_pos = get_cursor();
    row = cursor_pos / KBD_WIDTH;
    col = cursor_pos % KBD_WIDTH;

    if (shell_banner && !force) {
        shell_banner = vga_shellbanner(ch);
        return OK;
    }

    switch (ch) {
        case TY_NEWLINE:
            row += 1;
            break;

        case TY_RETURN:
            (*disp)[row][col] = 0;
            set_cursor(row * KBD_WIDTH);
            return OK;

        case TY_ESC:
            if (!shell_banner) {
                shell_banner = TRUE;
                vga_shellbanner(ch);
                return OK;
            }
            break;

        case TY_TAB:
            col += 4;
            break;

        case TY_BACKSP:
        case TY_BACKSP2:
            return OK;

        default:
            (*disp)[row][col] = ch | color;
            col++;
            break;
    }

    //overflow
    if (col >= KBD_WIDTH) {
        col = 0;
        row++;
    }
    //rollback
    if (row == KBD_HEIGHT) {
        for (i = 1; i < KBD_HEIGHT; i++) {
            memcpy((*disp)[i - 1], (*disp)[i], sizeof(uint16[KBD_WIDTH]));
        }
        memset((*disp)[KBD_HEIGHT - 1], 0, sizeof(uint16[KBD_WIDTH]));
        row = KBD_HEIGHT - 1;
    }

    set_cursor(row * KBD_WIDTH + col);
    (*disp)[row][col] = ' ' | color;
    return OK;
}

devcall vga_erase(bool8 invisible) {
    uint16 cursor_pos, row, col;

    cursor_pos = get_cursor();
    row = cursor_pos / KBD_WIDTH;
    col = cursor_pos % KBD_WIDTH;

    (*disp)[row][col] = 0;
    if (col != 0) {
        col--;
    } else {
        if (row != 0) {
            row--;
            col = KBD_WIDTH - 1;
        }
    }
    // Update cursor position
    set_cursor(row * KBD_WIDTH + col);
    // Set the character at the new cursor position to a space with the current color
    (*disp)[row][col] = ' ' | color;

    // If invisible is true, recursively erase the previous character
    if (invisible) {
        vga_erase(FALSE);
    }

    return OK;
}

bool8 vga_shellbanner(char ch) {
    static char buffer[64];
    static char *it = buffer;
    static int16 color_map[] = {0x0, 0x4, 0x2, 0xE, 0x1, 0x5, 0x3, 0xF};

    int32 opcode;

    *it++ = ch;

    // Return if the character is not 'm', indicating an incomplete sequence
    if (ch != 'm') return TRUE;

    // Handle single digit opcode
    if (buffer[3] == 'm') {           // BAN9 first digit
        opcode = buffer[2] - '0';
        if (opcode == 0)
            color = 0x0700;           // Reset color
        else if (opcode == 1)
            color |= 0x0800;          // Set bold
    }
    // Handle two digit opcode
    else if (buffer[4] == 'm') {      // BAN0 & BAN9 second digit
        opcode = (buffer[2] - '0') * 10 + (buffer[3] - '0');
        if (opcode >= 30 && opcode <= 37)
            color = (color & 0xF8FF) | (color_map[opcode - 30] << 8);   // Set text color
        else if (opcode >= 40 && opcode <= 47)
            color = (color & 0x0FFF) | (color_map[opcode - 40] << 12);  // Set background color
    }
    it = buffer;
    return FALSE;
}