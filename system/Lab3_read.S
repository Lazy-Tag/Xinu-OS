/* u2021201780_read.S - read (for x86) */
    .globl	u2021201780_read

u2021201780_read:
    movl 12(%esp), %edx
    movl 8(%esp), %esi
    movl 4(%esp), %edi
    int $54
    ret