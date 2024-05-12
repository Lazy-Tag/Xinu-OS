/* u2021201780_addargs.S - addargs (for x86) */
    .globl	u2021201780_addargs

u2021201780_addargs:
    movl %esp, %edi
    add $4, %edi
    int $58
    ret
