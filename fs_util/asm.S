	.globl uprintf
uprintf:
	pushal
	movl	$0, %eax
	int     $63
	popal
	ret

	.globl usleepms
usleepms:
	pushal
	movl	$3, %eax 
	movl 	36(%esp), %ebx 
	int 	$63
	popal
	ret
