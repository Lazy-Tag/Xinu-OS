/* u2021201780_printf.S - printf (for x86) */
    .globl	u2021201780_printf

u2021201780_printf:
    movl 8(%esp), %esi
    movl 4(%esp), %edi
    movl %esp, %edx
    addl $8, %edx
    movl %edx, %ecx
    movl %esi, %eax
    imull $4, %eax
    addl %eax, %edx
    int $48
    ret
