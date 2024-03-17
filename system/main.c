/*  main.c  - main */

#include <xinu.h>

void sndA(void);
void sndB(void);

process main(void)
{
    // struct  procent *prptr = &proctab[getpid()];
    // prptr->prprio = 20;
    // resume(create(sndA, 1024, 20, "process 1", 0));
    // resume(create(sndB, 1024, 20, "process 2", 0));
    /* Run the Xinu shell */

    recvclr();
    resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
    /* Wait for shell to exit and recreate it */

    while (TRUE) {
        receive();
        sleepms(200);
        kprintf("\n\nMain process recreating shell\n\n");
        resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
    }
  return OK;
}

void sndA(void) {
    kprintf("\n2021201780\nBowen Hu\nsndA\n");
    while(1) { putc(CONSOLE, 'A');}
}

void sndB(void) {
    kprintf("\n2021201780\nBowen Hu\nsndB\n");
    while(1) { putc(CONSOLE, 'B');}
}


