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
        u2021201780_printf("Usage: %s\n\n", 1, argv[0]);
        u2021201780_printf("Description:\n", 0);
        u2021201780_printf("\tImplement variable parameter functions and asynchronous event functions\n", 0);
        u2021201780_printf("\t-2.1\tlab2 2.1 test\n", 0);
        u2021201780_printf("\t-2.3\tlab2 2.2 test\n", 0);
        u2021201780_printf("\t-2.3\tlab2 2.3 test\n", 0);
    } else if (argc < 2) {
        u2021201780_fprintf(stderr, 1, "too few arguments\n");
        u2021201780_fprintf(stderr, 1, "Try '%s --help' for more information\n", argv[0]);
        return 1;
    } else if (argc >= 3) {
        u2021201780_fprintf(stderr, 1, "too many arguments\n");
        u2021201780_fprintf(stderr, 1, "Try '%s --help' for more information\n", argv[0]);
        return 1;
    }

    if (!strncmp(argv[1], "-2.1", 4) || !strncmp(argv[1], "-2.2", 4)) {
        u2021201780_FunPtr func;
        if (!strncmp(argv[1], "-2.1", 4))
            func = u2021201780_printf_1;
        else
            func = u2021201780_printf_2;

        u2021201780_printf("test: lab2 %s\n", 1, argv[1]);
        u2021201780_printf("we use 0 as end descriptor\n", 0);
        u2021201780_printf("test1: Test positive number\n", 0);
        u2021201780_printf("Expect nums: 6\tinput integer: 10 20 30 40 50 60 0\n", 0);
        func(10, 20, 30, 40, 50, 60, 0);
        u2021201780_printf("\n", 0);
        u2021201780_printf("test2: Test 1 number\n", 0);
        u2021201780_printf("Expect nums: 1\tinput integer: 10 0\n", 0);
        func(10, 0);
        u2021201780_printf("\n", 0);
        u2021201780_printf("test3: Test negative number\n", 0);
        u2021201780_printf("Expect nums: 4\tinput integer: -10 -20 -30 -40 0\n", 0);
        func(-10, -20, -30, -40, 0);
        u2021201780_printf("\n", 0);
        u2021201780_printf("test4: End at the first number\n", 0);
        u2021201780_printf("Expect nums: 3\tinput integer: 0 10 20 0\n", 0);
        func(0, 10, 20, 0);
        u2021201780_printf("\n", 0);
        u2021201780_printf("test5: End at halfway\n", 0);
        u2021201780_printf("Expect nums: 5\tinput integer: 10 20 30 0 50 0\n", 0);
        func(10, 20, 30, 0, 50, 0);
        u2021201780_printf("\n", 0);
    }

    if (!strncmp(argv[1], "-2.3", 4)) {
        u2021201780_printf("test: lab2 %s\n", 1, argv[1]);
        u2021201780_printf("main(1): %d.%d\n", 2, clktime, count1000);
        u2021201780_delay_run(3, u2021201780_delay_test, 10, 20, 0);
        u2021201780_printf("main(2): %d.%d\n", 2, clktime, count1000);
        u2021201780_delay_run(4, u2021201780_delay_test, -10, -20, 0);
        u2021201780_printf("main(3): %d.%d\n", 2, clktime, count1000);
        u2021201780_delay_run(5, u2021201780_delay_test, 40, 50, 0);
    }

    u2021201780_printf("Name: Bowen Hu\tstudent ID: 2021201780\n", 0);

    return 0;
}


void u2021201780_printf_1(int first, ...) {
    u2021201780_printf("Name: Bowen Hu\tstudent ID: 2021201780\tFunc Name: u2021201780_printf_1\n", 0);
    if (first == 0) return ;
    u2021201780_printf("%d ", 1, first);
    va_list args;
    va_start(args, first);

    int next;
    while (1) {
        next = va_arg(args, int);
        if (next == 0) break;
        u2021201780_printf("%d ", 1, next);
    }

    va_end(args);
}

void u2021201780_printf_2(int first, ...) {
    u2021201780_printf("Name: Bowen Hu\tstudent ID: 2021201780\tFunc Name: u2021201780_printf_2\n", 0);
    if (first == 0) return ;
    u2021201780_printf("%d ", 1, first);
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
        u2021201780_printf("%d ", 1, next);
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
    u2021201780_resume(u2021201780_create((u2021201780_FunPtr)func, 8192, 20, "delay_func", 6, seconds,
                  arg_list[0], arg_list[1], arg_list[2], arg_list[3], arg_list[4]));

    va_end(args);
}

void u2021201780_delay_test(int seconds, int a, int b, int c, int d, int e) {
    u2021201780_sleep(seconds);
    u2021201780_printf("delay_test: %d.%d\n", 2, clktime, count1000);
    u2021201780_printf_1(a, b, c, d, e);
    u2021201780_printf("\n", 0);
}
