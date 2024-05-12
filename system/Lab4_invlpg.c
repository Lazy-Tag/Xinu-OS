/* invlpg.c - invalid page entry in TLB */
#include <xinu.h>

void invlpg(void *va)
{
	asm volatile (
		"invlpg (%0)"
		:
		: "r"(va)
		: "memory"
	);
}