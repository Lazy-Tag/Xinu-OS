/* xsh_lab3.c - xsh_test_kenerl_user */

#include <xinu.h>
#include <stdio.h>
#include <stdarg.h>

void k2021201780_printf(char *fmt, int count, ...) {
    va_list args;
    va_start(args, count);
    uint32 data[10];
    for (int i = 0; i < count; i ++ )
        data[i] = va_arg(args, uint32);
    printf(fmt, data[0], data[1], data[2], data[3],
                data[4], data[5], data[6], data[7],
                data[8], data[9]);
    va_end(args);
}

void k2021201780_fprintf(did32 dev, char *fmt, int count, ...) {
    va_list args;
    va_start(args, count);
    uint32 data[10];
    for (int i = 0; i < count; i ++ )
        data[i] = va_arg(args, uint32);
    fprintf(dev, fmt, data[0], data[1], data[2], data[3],
                data[4], data[5], data[6], data[7],
                data[8], data[9]);
    va_end(args);
}

pid32 k2021201780_create(uint32* str) {
    void* func = (void*)(*str++);
    uint32 size = (uint32)(*str++);
    uint32 pri = (uint32)(*str++);
    char* name = (char*)(*str++);
    uint32 nargs = (uint32)(*str++);
    int cnt = nargs - 1;
    while(cnt-- > 0)
        str++;
    pid32 pid = ucreate(func, size, pri, name, nargs, str);
    resume(pid);
    return pid;
}

void u2021201780_uptest(int a, int b) {
    int pid = getpid();
    struct procent *prptr = &proctab[pid];
    u2021201780_printf("name: %s proc=%d a=%d\n", 3, prptr->prname, pid, a);
    u2021201780_sleep(1);
    u2021201780_printf("name: %s proc=%d a=%d\n", 3, prptr->prname, pid, b);
}

shellcmd xsh_lab3(int argc, char* argv[]) {
    u2021201780_create(u2021201780_uptest, 8192, 20, "uptest1", 2, 2, 3);
    u2021201780_create(u2021201780_uptest, 8192, 20, "uptest2", 2, 4, 5);
    u2021201780_create(u2021201780_uptest, 8192, 20, "uptest3", 2, 6, 7);
    u2021201780_printf("2021201780\tBowen Hu\n", 0);
    return 0;
}
