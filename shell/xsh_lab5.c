/* xsh_lab5.c - xsh_test_VGA */

#include <xinu.h>

shellcmd xsh_lab5(int nargs, char *args[]) {
    int x;
    u2021201780_printf("&x=0x%x\n", 1, &x);
    for(int i = 0; i < nargs; i++)
        u2021201780_printf("arg-%d: %s\n", 2, i, args[i]);

    u2021201780_printf("2021201780 Bowen Hu\n", 0);

    u2021201780_printf("Test close to line overflow\n", 0);
    u2021201780_printf(BLUE_BAN, 0);
    for(int i = 0; i < 80; i++)
        u2021201780_printf("1", 0);
    u2021201780_printf(ORIGIN_BAN, 0);

    u2021201780_printf("Test line overflow\n", 0);
    u2021201780_printf(BLUE_BAN, 0);
    for(int i = 0; i < 120; i++)
        u2021201780_printf("1", 0);
    u2021201780_printf(ORIGIN_BAN, 0);

//    u2021201780_printf("Test screen overflow\n", 0);
//    for (int i = 0; i < 30; i ++ ) {
//        u2021201780_printf("%d: ", 1, i);
//        for(int j = 0; j < 26; j++)
//            u2021201780_printf("%c", 1, 'a' + j);
//        u2021201780_printf("\n", 0);
//    }

    return 0;
}