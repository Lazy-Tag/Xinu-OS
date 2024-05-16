/* xsh_lab4.c - xsh_test_kenerl_user */

#include <xinu.h>

shellcmd xsh_lab4(int argc, char* argv[]) {
    int x;
    u2021201780_printf("&x= 0x%x\n", 1, &x);
    u2021201780_printf("argc = %d\n", 1, argc);
    for (int i = 0; i < argc; i ++ ) {
        u2021201780_printf("arg-%d: %s\n", 2, i, argv[i]);
    }

//    int *p;
//    u2021201780_printf("call malloc\n", 0);
//    p = (int*) u2021201780_malloc(5 * sizeof(int));
//    for (int i = 0; i < 5; i ++ ) {
//        p[i] = i;
//    }
//
//    for (int i = 0; i < 5; i ++ )
//        u2021201780_printf("p[%d] = %d\n", 2, i, p[i]);
//
//    u2021201780_printf("call free\n", 0);
//    u2021201780_free(p, 5 * sizeof(int));
//
//    u2021201780_printf("call malloc\n", 0);
//    p = (int*) u2021201780_malloc(5 * sizeof(int));
//    for (int i = 0; i < 5; i ++ ) {
//        p[i] = i + 5;
//    }
//
//    for (int i = 0; i < 5; i ++ )
//        u2021201780_printf("p[%d] = %d\n", 2, i, p[i]);
//
//    u2021201780_printf("call free\n", 0);
//    u2021201780_free(p, 5 * sizeof(int));

    u2021201780_printf("2021201780\tBowen Hu\n", 0);
    return 0;
}