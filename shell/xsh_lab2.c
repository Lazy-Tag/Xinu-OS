#include <xinu.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef void(*u2021201780_FunPtr)(int, ...);

void u2021201780_printf_1(int first, ...);
void u2021201780_printf_2(int first, ...);
void u2021201780_printf_3(char* args);
void u2021201780_delay_run(int seconds, void* func, ...);
void u2021201780_delay_test(int seconds, int a, int b, int c, int d, int e);

shellcmd xsh_lab2(int argc, char* argv[]) {

    if (argc == 2 && strncmp(argv[1], "--help", 6) == 0) {
        kprintf("Usage: %s\n\n", argv[0]);
        kprintf("Description:\n");
        kprintf("\tImplement variable parameter functions and asynchronous event functions\n");
        kprintf("\t-2.1\tlab2 2.1 test\n");
        kprintf("\t-2.3\tlab2 2.2 test\n");
        kprintf("\t-2.3\tlab2 2.3 test\n");
    } else if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        fprintf(stderr, "Try '%s --help' for more information\n", argv[0]);
        return 1;
    } else if (argc >= 3) {
        fprintf(stderr, "too many arguments\n");
        fprintf(stderr, "Try '%s --help' for more information\n", argv[0]);
        return 1;
    }

    if (!strncmp(argv[1], "-2.1", 4) || !strncmp(argv[1], "-2.2", 4)) {
        u2021201780_FunPtr func;
        if (!strncmp(argv[1], "-2.1", 4))
            func = u2021201780_printf_1;
        else
            func = u2021201780_printf_2;

        kprintf("test: lab2 %s\n", argv[1]);
        kprintf("we use 0 as end descriptor\n");
        kprintf("test1: Test positive number\n");
        kprintf("Expect nums: 6\tinput integer: 10 20 30 40 50 60 0\n");
        func(10, 20, 30, 40, 50, 60, 0);
        kprintf("\n");
        kprintf("test2: Test 1 number\n");
        kprintf("Expect nums: 1\tinput integer: 10 0\n");
        func(10, 0);
        kprintf("\n");
        kprintf("test3: Test negative number\n");
        kprintf("Expect nums: 4\tinput integer: -10 -20 -30 -40 0\n");
        func(-10, -20, -30, -40, 0);
        kprintf("\n");
        kprintf("test4: End at the first number\n");
        kprintf("Expect nums: 3\tinput integer: 0 10 20 0\n");
        func(0, 10, 20, 0);
        kprintf("\n");
        kprintf("test5: End at halfway\n");
        kprintf("Expect nums: 5\tinput integer: 10 20 30 0 50 0\n");
        func(10, 20, 30, 0, 50, 0);
        kprintf("\n");
    }

    if (!strncmp(argv[1], "-2.3", 4)) {
        kprintf("test: lab2 %s\n", argv[1]);
        printf("main(1): %d.%d\n", clktime, count1000);
        u2021201780_delay_run(3, u2021201780_delay_test, 10, 20, 0);
        printf("main(2): %d.%d\n", clktime, count1000);
        u2021201780_delay_run(4, u2021201780_delay_test, -10, -20, 0);
        printf("main(3): %d.%d\n", clktime, count1000);
        u2021201780_delay_run(5, u2021201780_delay_test, 40, 50, 0);
    }

    kprintf("Name: Bowen Hu\tstudent ID: 2021201780\n");

    return 0;
}


void u2021201780_printf_1(int first, ...) {
    kprintf("Name: Bowen Hu\tstudent ID: 2021201780\tFunc Name: u2021201780_printf_1\n");
    if (first == 0) return ;
    kprintf("%d ", first);
    va_list args;
    va_start(args, first);

    int next;
    while (1) {
        next = va_arg(args, int);
        if (next == 0) break;
        kprintf("%d ", next);
    }

    va_end(args);
}

void u2021201780_printf_2(int first, ...) {
    kprintf("Name: Bowen Hu\tstudent ID: 2021201780\tFunc Name: u2021201780_printf_2\n");
    if (first == 0) return ;
    kprintf("%d ", first);
    va_list args;
    va_start(args, first);

    u2021201780_printf_3(args);
    va_end(args);
}

void u2021201780_printf_3(char* args) {
    int next;
    while (1) {
        next = va_arg(args, int);
        if (next == 0)
            break;
        kprintf("%d ", next);
    }
}

void u2021201780_delay_run(int seconds, void* func, ...) {
    va_list args;
    va_start(args, func);
    int arg_list[5] = {0};
    for (int i = 0; i < 5; i ++ ) {
        if ((arg_list[i] = va_arg(args, int)) == 0)
            break;
    }
    resume(create((u2021201780_FunPtr)func, 8192, 20, "delay_func", 6, seconds,
                  arg_list[0], arg_list[1], arg_list[2], arg_list[3], arg_list[4]));

    va_end(args);
}

void u2021201780_delay_test(int seconds, int a, int b, int c, int d, int e) {
    sleep(seconds);
    printf("delay_test: %d.%d\n", clktime, count1000);
    u2021201780_printf_1(a, b, c, d, e);
    kprintf("\n");
}
