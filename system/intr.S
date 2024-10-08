/* intr.s - disable, restore, enable, pause, halt, spurious_irq7,	*/
/*			spurious_irq15					*/

#include <icu.s>
	.data
	.globl	_saveint
_saveint:	.long

	.text
	.globl	disable
	.globl	restore
	.globl	enable
	.globl	pause
	.globl	halt
	.globl	spurious_irq7
	.globl	spurious_irq15

/*------------------------------------------------------------------------
 * disable  -  Disable interrupts and return the previous state
 *------------------------------------------------------------------------
 */
disable:
	pushfl
	cli
	popl	%eax
	andl	$0x00000200,%eax
	ret

/*------------------------------------------------------------------------
 * restore  -  Restore interrupts to value given by mask argument
 *------------------------------------------------------------------------
 */
restore:
        cli
        movw    4(%esp),%ax
	andl	$0x00000200,%eax
	pushl	%eax
	popfl
        ret

/*------------------------------------------------------------------------
 * enable  -  Enable all interrupts
 *------------------------------------------------------------------------
 */
enable:
	sti
	ret

/*------------------------------------------------------------------------
 * pause  -  Halt the processor until an interrupt occurs
 *------------------------------------------------------------------------
 */
pause:
	call enable
	hlt
	ret


/*------------------------------------------------------------------------
 * halt  -  Do nothing forever
 *------------------------------------------------------------------------
 */
halt:
	jmp halt
	ret

/*------------------------------------------------------------------------
 * spurious_irq7  -  Handle an unexpected interrupt for irq7
 *------------------------------------------------------------------------
 */
spurious_irq7:
	mov	$EOI,%al
	outb	%al,$OCW1_2
	iret

/*------------------------------------------------------------------------
 * spurious_irq15  -  Handle an unexpected interrupt for irq15
 *------------------------------------------------------------------------
 */
spurious_irq15:
	mov	$EOI,%al
	outb	%al,$OCW1_2
	mov	$EOI,%al
	outb	%al,$OCW2_2
	iret

/*------------------------------------------------------------------------
 * Xtrap  -  Entry point when no interrupt/exception handler in place
 *------------------------------------------------------------------------
 */
Xtrap:
	call	trap			/* Call C trap handler		*/
	iret				/* Should never occur		*/

/************************************************************************/
/*									*/
/* Definitions of _Xint0 through _Xint47 entry points for interrupts	*/
/*		and exceptions (referenced as initial entries in the	*/
/*		interrupt vector array below)				*/
/*									*/
/************************************************************************/

	.globl	_Xint0			/* Divide error			*/
_Xint0:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$0
	jmp	Xtrap

	.globl  _Xint1			/* Debug exception		*/
_Xint1:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$1
	jmp	Xtrap

	.globl  _Xint2			/* Nonmaskable interrupt	*/
_Xint2:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$2
	jmp	Xtrap

	.globl  _Xint3			/* Breakpoint (one-byte INT 3	*/
_Xint3:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$3
	jmp	Xtrap

	.globl  _Xint4			/* Overflow (INTO instruction)	*/
_Xint4:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$4
	jmp	Xtrap

	.globl  _Xint5			/* Bounds check (BOUND instr)	*/
_Xint5:
	pushl	%ebp
	movl	%esp,%ebp
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$5
	jmp	Xtrap

	.globl  _Xint6			/* Invalid opcode		*/
_Xint6:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$6
	jmp	Xtrap

	.globl  _Xint7			/* Coprocessor not available	*/
_Xint7:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$7
	jmp	Xtrap

	.globl  _Xint8			/* Double fault			*/
_Xint8:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$8
	jmp	Xtrap
	
	.globl  _Xint9			/* (reserved)			*/
_Xint9:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$9
	jmp	Xtrap

	.globl  _Xint10			/* Invalid TSS			*/
_Xint10:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$10
	jmp	Xtrap

	.globl  _Xint11			/* Segment not present		*/
_Xint11:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$11
	jmp	Xtrap

	.globl  _Xint12			/* Stack exception		*/
_Xint12:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$12
	jmp	Xtrap

	.globl  _Xint13			/* General protection		*/
_Xint13:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$13
	jmp	Xtrap

	.globl  _Xint14			/* Page fault			*/
_Xint14:
    /* Lab4 2021201780:Begin */
	pushal
	pushl 0x24(%esp)
	movl 0x34(%esp), %edi

	pushl %edi
	call _do_stk_expand
    pop %edi
	ret
	/* Lab4 2021201780:End */

	.globl  _Xint15			/* (reserved)			*/
_Xint15:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$15
	jmp	Xtrap

	.globl  _Xint16			/* Coprocessor error		*/
_Xint16:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$16
	jmp	Xtrap

	.globl  _Xint17			/* (reserved)			*/
_Xint17:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$17
	jmp	Xtrap

	.globl  _Xint18			/* (reserved)			*/
_Xint18:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$18
	jmp	Xtrap

	.globl  _Xint19			/* (reserved)			*/
_Xint19:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$19
	jmp	Xtrap

	.globl  _Xint20			/* (reserved)			*/
_Xint20:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$20
	jmp	Xtrap

	.globl  _Xint21			/* (reserved)			*/
_Xint21:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$21
	jmp	Xtrap

	.globl  _Xint22			/* (reserved)			*/
_Xint22:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$22
	jmp	Xtrap

	.globl  _Xint23			/* (reserved)			*/
_Xint23:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$23
	jmp	Xtrap

	.globl  _Xint24			/* (reserved)			*/
_Xint24:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$24
	jmp	Xtrap

	.globl  _Xint25			/* (reserved)			*/
_Xint25:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$25
	jmp	Xtrap

	.globl  _Xint26			/* (reserved)			*/
_Xint26:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$26
	jmp	Xtrap

	.globl  _Xint27			/* (reserved)			*/
_Xint27:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$27
	jmp	Xtrap

	.globl  _Xint28			/* (reserved)			*/
_Xint28:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$28
	jmp	Xtrap

	.globl  _Xint29			/* (reserved)			*/
_Xint29:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$29
	jmp	Xtrap

	.globl  _Xint30			/* (reserved)			*/
_Xint30:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$30
	jmp	Xtrap

	.globl  _Xint31			/* (reserved)			*/
_Xint31:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$31
	jmp	Xtrap

	.globl  _Xint32			/* (reserved)			*/
_Xint32:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$32
	jmp	Xtrap

	.globl  _Xint33			/* (reserved)			*/
_Xint33:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$33
	jmp	Xtrap

	.globl  _Xint34			/* (reserved)			*/
_Xint34:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$34
	jmp	Xtrap

	.globl  _Xint35			/* (reserved)			*/
_Xint35:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$35
	jmp	Xtrap

	.globl  _Xint36			/* (reserved)			*/
_Xint36:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$36
	jmp	Xtrap

	.globl  _Xint37			/* (reserved)			*/
_Xint37:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$37
	jmp	Xtrap

	.globl  _Xint38			/* (reserved)			*/
_Xint38:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$38
	jmp	Xtrap

	.globl  _Xint40			/* (reserved)			*/
_Xint40:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$40
	jmp	Xtrap

	.globl  _Xint41			/* (reserved)			*/
_Xint41:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$41
	jmp	Xtrap

	.globl  _Xint42			/* (reserved)			*/
_Xint42:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$42
	jmp	Xtrap

	.globl  _Xint43			/* (reserved)			*/
_Xint43:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$43
	jmp	Xtrap

	.globl  _Xint44			/* (reserved)			*/
_Xint44:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$44
	jmp	Xtrap

	.globl  _Xint45			/* (reserved)			*/
_Xint45:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$45
	jmp	Xtrap

	.globl  _Xint46			/* (reserved)			*/
_Xint46:
	pushal
	movl	%esp, %eax
	pushl	%eax
	pushl	$46
	jmp	Xtrap
/*Lab3 2021201780:Begin*/
	.globl  _Xint48
_Xint48:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

LO:
    cmp %edx, %ecx
    je L1
    push (%edx)
    subl $4, %edx
    jmp LO
L1:
	push %esi
	push %edi
	call k2021201780_printf
    pop %edi
    pop %esi
    movl %esi, %eax
L3:
    cmp $0, %eax
    je L4
    pop %ebx
    subl $1, %eax
    jmp L3
L4:
    pop %ebx

	popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint49
_Xint49:
	jmp userret

	.globl  _Xint50
_Xint50:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
    
    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	push %edi
	call sleep
    pop %edi

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint51
_Xint51:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	push %ebx
    movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

    addl $0x10, %edi
    movl (%edi), %eax
    movl %eax, %ebx
    imull $4, %eax
    addl %eax, %edi
L12:
	testl %eax, %eax
	je L11
	pushl (%edi)
	subl $4, %edi
	subl $4, %eax
	jmp L12
L11:
	pushl %ebx
	subl $4, %edi
	pushl (%edi)
	subl $4, %edi
	pushl (%edi)
	subl $4, %edi
	pushl (%edi)
	subl $4, %edi
	pushl (%edi)

    call create

    pop %edi
    pop %edi
    pop %edi
    pop %edi
    pop %edi

    imull $4, %edi
    addl %edi, %esp

    pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

    .globl  _Xint52
_Xint52:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	push %edi
	call sleepms
    pop %edi

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint53
_Xint53:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
L5:
    cmp %ecx, %eax
    je L6
    push (%ecx)
    subl $4, %ecx
    jmp L5
L6:
	movl $(0x2 << 3), %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

    push %edx
	push %esi
	push %edi
	call k2021201780_fprintf
    pop %edi
    pop %esi
    pop %edx
    movl %edx, %eax
L7:
    cmp $0, %eax
    je L8
    pop %ebx
    subl $1, %eax
    jmp L7
L8:
    pop %ebx
	popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint54
_Xint54:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	movl $(0x2 << 3), %eax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

    push %edx
	push %esi
	push %edi
	call read
    pop %edi
    pop %esi
    pop %edx

	popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	    .globl  _Xint55
_Xint55:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	push %edi
	call resume
    pop %edi

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint56
_Xint56:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	call receive

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint57
_Xint57:
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	call recvclr

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

/*Lab3 2021201780:End*/

/*Lab4 2021201780:Begin*/
	.globl  _Xint58			/* Syscalls			*/
_Xint58:
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    push %ebx
    movl $(0x2 << 3), %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs

    movl $6, %eax
    addl $20, %edi
    movl %eax, %ebx
L9:
    testl %eax, %eax
    je L10
    pushl (%edi)
    subl $4, %edi
    subl $1, %eax
    jmp L9
L10:
    call addargs
    leal	(%esp, %ebx, 4), %esp
    pop %ebx

    popl %gs
    popl %fs
    popl %es
    popl %ds
    iret

    .globl  _Xint59
_Xint59:
    pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	push %edi
	call mm_malloc
    pop %edi

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

   .globl  _Xint60
_Xint60:
    pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	push %edi
	call mm_free
	pop %edi

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret

	.globl  _Xint61
_Xint61:
    pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

    push %ebx
	movl $(0x2 << 3), %ebx
	movw %bx, %ds
	movw %bx, %es
	movw %bx, %fs
	movw %bx, %gs

	call mem_init

	pop %ebx
    popl %gs
	popl %fs
	popl %es
	popl %ds
	iret
/*Lab4 2021201780:End*/


/*------------------------------------------------------------------------
 * _Xint? - array of exception/interrupt entry points that are
 *		initialized to point to default handlers
 *------------------------------------------------------------------------
 */
	.globl	defevec
defevec:
	.long	_Xint0			/* Divide error			*/
	.long	_Xint1			/* Debug exceptions		*/
	.long	_Xint2			/* Nonmaskable interrupt	*/
	.long	_Xint3			/* Breakpoint (one-byte INT 3	*/
					/*    instruction)		*/	
	.long	_Xint4			/* Overflow (INTO instruction)	*/
	.long	_Xint5			/* Bounds check (BOUND instr)	*/
	.long	_Xint6			/* Invalid opcode		*/
	.long	_Xint7			/* Coprocessor not available	*/
	.long	_Xint8			/* Double fault			*/
	.long	_Xint9			/* (reserved)			*/
	.long	_Xint10			/* Invalid TSS			*/
	.long	_Xint11			/* Segment not present		*/
	.long	_Xint12			/* Stack exception		*/
	.long	_Xint13			/* General protection		*/
	.long	_Xint14			/* Page fault			*/
	.long	_Xint15			/* (reserved)			*/
	.long	_Xint16			/* Coprocessor error		*/
	.long	_Xint17			/* (reserved)			*/
	.long	_Xint18			/* (reserved)			*/
	.long	_Xint19			/* (reserved)			*/
	.long	_Xint20			/* (reserved)			*/
	.long	_Xint21			/* (reserved)			*/
	.long	_Xint22			/* (reserved)			*/
	.long	_Xint23			/* (reserved)			*/
	.long	_Xint24			/* (reserved)			*/
	.long	_Xint25			/* (reserved)			*/
	.long	_Xint26			/* (reserved)			*/
	.long	_Xint27			/* (reserved)			*/
	.long	_Xint28			/* (reserved)			*/
	.long	_Xint29			/* (reserved)			*/
	.long	_Xint30			/* (reserved)			*/
	.long	_Xint31			/* (reserved)			*/
	.long	_Xint32			/* available for ext. interrupt	*/
	.long	_Xint33			/* available for ext. interrupt	*/
	.long	_Xint34			/* available for ext. interrupt	*/
	.long	_Xint35			/* available for ext. interrupt	*/
	.long	_Xint36			/* available for ext. interrupt	*/
	.long	_Xint37			/* available for ext. interrupt	*/
	.long	_Xint38			/* available for ext. interrupt	*/
	.long	spurious_irq7
	.long	_Xint40			/* available for ext. interrupt	*/
	.long	_Xint41			/* available for ext. interrupt	*/
	.long	_Xint42			/* available for ext. interrupt	*/
	.long	_Xint43			/* available for ext. interrupt	*/
	.long	_Xint44			/* available for ext. interrupt	*/
	.long	_Xint45			/* available for ext. interrupt	*/
	.long	_Xint46			/* available for ext. interrupt	*/
	.long	spurious_irq15
	/*Lab3 2021201780:Begin*/
	.long	_Xint48			/* my interrupt	for printf*/
	.long   _Xint49         /* my interrupt	for u2k*/
	.long   _Xint50         /* my interrupt	for sleep*/
	.long   _Xint51         /* my interrupt	for create*/
	.long   _Xint52         /* my interrupt	for sleepms*/
	.long   _Xint53         /* my interrupt	for fprintf*/
	.long   _Xint54         /* my interrupt	for read*/
	.long   _Xint55         /* my interrupt	for resume*/
	.long   _Xint56         /* my interrupt	for receive*/
	.long   _Xint57         /* my interrupt	for recvclr*/
	/*Lab3 2021201780:End*/
	/*Lab4 2021201780:Begin*/
	.long   _Xint58 /* my interrupt	for add_args*/
	.long   _Xint59 /* my interrupt	for mm_malloc*/
	.long   _Xint60 /* my interrupt	for mm_free*/
	.long   _Xint61 /* my interrupt	for mm_init*/
	/*Lab4 2021201780:End*/
