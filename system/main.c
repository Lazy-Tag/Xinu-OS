/*  main.c  - main */

#include <xinu.h>

void sndA() {
	while(1) {
		putc(CONSOLE, 'A');
	}
}

void sndB() {
	while(1) {
		putc(CONSOLE, 'B');
	}
}

process	main(void)
{

	/* Run the Xinu shell */
    /*Lab3 2021201780:Begin*/
	u2021201780_recvclr();
    u2021201780_resume(u2021201780_create(shell, 8192, 50, "shell", 1, CONSOLE));

	//resume(create(sndA, 4096, 20, "processA", 1, CONSOLE));
	//resume(create(sndB, 4096, 20, "processB", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		u2021201780_receive();
		u2021201780_sleepms(200);
		u2021201780_printf("\n\nMain process recreating shell\n\n", 0);
		u2021201780_resume(u2021201780_create(shell, 4096, 20, "shell", CONSOLE));
	}

    /*Lab3 2021201780:End*/
	return OK;
    
}
