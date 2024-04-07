/* u2021201780_create.S - resume (for x86) */
    .globl	u2021201780_resume

u2021201780_resume:
    movl 4(%esp), %edi
    int $55
    ret