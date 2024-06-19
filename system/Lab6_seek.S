/* seek.S -  User mode seek */

.globl	u2021201780_seek

u2021201780_seek:
    movl 0x8(%esp), %esi
    movl 0x4(%esp), %edi
    int $62
    ret