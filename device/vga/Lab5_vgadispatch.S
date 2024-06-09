/* Lab5 vgadispatch.S - kbddisp */

#include <icu.s>

		.text
		.globl  vgadispatch
		.globl	vgahandler

vgadispatch:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2

		call	vgahandler	/* Call the handler		*/
		
		sti
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/