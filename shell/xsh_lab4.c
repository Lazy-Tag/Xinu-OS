/* xsh_lab4.c - xsh_test_kenerl_user */

#include <xinu.h>

shellcmd xsh_lab4(int argc, char* argv[]) {
    int x;
    u2021201780_printf("&x= 0x%x\n", 1, &x);
    for (int i = 0; i < argc; i ++ ) {
        u2021201780_printf("arg-%d: %s\n", 2, i, argv[i]);
    }
    u2021201780_printf("2021201780\tBowen Hu\n", 0);
    return 0;
}