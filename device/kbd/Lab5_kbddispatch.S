/* Lab5 kbd_dispatch.S - kbddisp */

#include <icu.s>

		.text
		.globl  kbd_dispatch
		.globl	kbd_handler

kbd_dispatch:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2

		call	kbd_handler	/* Call the handler		*/
		
		sti
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/