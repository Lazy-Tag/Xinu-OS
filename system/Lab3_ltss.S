/* k2021201780_ltss.S - ltss (for x86) */
    .globl	k2021201780_ltss

k2021201780_ltss:
    movl 4(%esp), %eax
    ltr %ax
    ret
