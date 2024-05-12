/* u2021201780_sleep.S - sleep (for x86) */
    .globl	u2021201780_sleep

u2021201780_sleep:
    movl 4(%esp), %edi
    int $50
    ret