/* u2021201780_create.S - create (for x86) */
    .globl	u2021201780_create

u2021201780_create:
    movl %esp, %edi
    add $4, %edi
    int $51
    ret