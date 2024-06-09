#include <icu.s>

		.text
		.globl  multidispatch
		.globl  multihandler

/*------------------------------------------------------------------------
 *  multidispatch  -  Dispatch an interrupt to the multi handler function
 *------------------------------------------------------------------------
 */

multidispatch:
		pushal			/* Save general-purpose regs.	*/
		pushfl			/* Save the flags register	*/
		cli			/* Prevent further interrupts	*/
		movb	$EOI,%al	/* Clear the interrupt		*/
		outb	%al,$OCW1_2

        call multihandler
        call ttyhandler

		sti
		popfl			/* Restore the flags register	*/
		popal			/* Restore general-purpose regs.*/
		iret			/* Return from interrupt	*/