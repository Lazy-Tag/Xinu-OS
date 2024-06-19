/* load_code.S -  User mode load code */

.globl	u2021201780_load_code

u2021201780_load_code:
    movl 0x8(%esp), %esi
    movl 0x4(%esp), %edi
    int $63
    ret