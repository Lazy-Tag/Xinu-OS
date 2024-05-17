/* malloc_free.S -  User mode malloc and free */
    .globl	u2021201780_malloc
    .globl	u2021201780_free
    .globl	u2021201780_heap_init

u2021201780_malloc:
    movl 0x4(%esp), %edi
    int $59
    ret

u2021201780_free:
    movl 0x4(%esp), %edi
    int $60
    ret

u2021201780_heap_init:
    movl 0x4(%esp), %edi
    int $61
    ret