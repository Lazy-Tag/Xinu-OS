/* k2021201780_fprintf.S - fprintf (for x86) */
    .globl	u2021201780_fprintf

u2021201780_fprintf:
    movl 12(%esp), %edx
    movl 8(%esp), %esi
    movl 4(%esp), %edi
    movl %esp, %ecx
    addl $12, %ecx
    movl %edx, %eax
    imull $4, %eax
    addl %eax, %ecx
    movl %esp, %eax
    addl $12, %eax
    int $53
    ret
