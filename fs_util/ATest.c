extern void uprintf(char*, ...);

extern void uprintf(char*, ...);

extern void uread(int, char*, int);

extern void uwrite(int, char*, int);

extern void useek(int, int);

int a = 5;

int ATest(int argc, char* argv[]) {
    int x = argc;
    uprintf("&a = 0x%x a = %d\n", 2, &a, a);
    uprintf("&x=0x%x, &ATest=0x%x\n", 2, &x, &ATest);
    for (int i = 0; i < argc; i++) {
        uprintf("arg-%d: %s\n", 2, i, argv[i]);
    }
    uprintf("2021201780\tBowen Hu\n", 0);
    return 0;
}



