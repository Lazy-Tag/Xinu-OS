/* xsh_lab4.c - xsh_test_kenerl_user */

#include <xinu.h>

shellcmd xsh_lab4(int argc, char* argv[]) {
    u2021201780_heap_init();
    int x;
    u2021201780_printf("&x= 0x%x\n", 1, &x);
    u2021201780_printf("argc = %d\n", 1, argc);
    for (int i = 0; i < argc; i ++ ) {
        u2021201780_printf("arg-%d: %s\n", 2, i, argv[i]);
    }

    u2021201780_printf("call malloc int p[5]:\n", 0);
    int *p = (int*) u2021201780_malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i ++ ) {
        p[i] = i;
    }

    for (int i = 0; i < 5; i ++ )
        u2021201780_printf("p[%d] = %d ", 2, i, p[i]);

    u2021201780_printf("\ncall malloc int q[10]:\n", 0);
    int *q = (int*) u2021201780_malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i ++ ) {
        q[i] = i * 2;
    }
    for (int i = 0; i < 10; i ++ )
        u2021201780_printf("q[%d] = %d ", 2, i, q[i]);

    u2021201780_printf("\ncall free\n", 0);
    u2021201780_free(p);
    u2021201780_free(q);

    u2021201780_printf("2021201780\tBowen Hu\n", 0);
    return 0;
}