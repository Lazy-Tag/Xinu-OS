/* k2021201780_sleep.S - sleepms (for x86) */
    .globl	u2021201780_sleepms

u2021201780_sleepms:
    movl 4(%esp), %edi
    int $52
    ret