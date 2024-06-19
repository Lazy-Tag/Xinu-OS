    .globl	uprintf

uprintf:
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

    .globl	uread

uread:
    movl 12(%esp), %edx
    movl 8(%esp), %esi
    movl 4(%esp), %edi
    int $54
    ret

    .globl	uwrite

uwrite:
    movl 12(%esp), %edx
    movl 8(%esp), %esi
    movl 4(%esp), %edi
    int $64
    ret

    .globl	useek

useek:
    movl 0x8(%esp), %esi
    movl 0x4(%esp), %edi
    int $62
    ret