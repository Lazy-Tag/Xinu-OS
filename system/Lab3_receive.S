/* k2021201780_printf.S - receive (for x86) */
    .globl	u2021201780_receive

u2021201780_receive:
    int $56
    ret
