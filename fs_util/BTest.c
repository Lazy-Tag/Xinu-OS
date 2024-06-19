typedef struct {
	int offset;
	int size;
	char   name[128];
}File;

extern void uprintf(char*, ...);

extern void uread(int, char*, int);

extern void uwrite(int, char*, ...);

extern void useek(int, int);

#define DISK 5

int BTest(int argc, char* argv[]) {
    useek(DISK, 0);
    if (argc < 3) {
        uprintf("Usage: BTest <input file> <input string>\n", 0);
        return 1;
    }
    char* file_name = argv[1];
    File dir, file;
    uread(DISK, (char*)&dir, sizeof(File));
    int flag = 0, offset;
    for (int i = 0; i < dir.size; i++) {
        uread(DISK, (char*)&file, sizeof(File));
        int equal = 1;
        char *a = file_name, *b = file.name;
        while (*a && *b) {
            if (*a != *b) {
                equal = 0;
                break;
            }
            a++; b++;
        }
        if (equal) {
            offset = file.offset;
            flag = 1;
            break;
        }
    }

    if (!flag) {
        uprintf("File not found\n", 0);
        return 1;
    }

    char buf[128];
    int len; for (len = 0; argv[2][len]; len++);
//    useek(DISK, offset);
//    uread(DISK, buf, 12);
//    uprintf("Read from disk after write: %s\n", 1, buf);

    useek(DISK, offset);
    uwrite(DISK, argv[2], len);
    uprintf("Write to disk: %s\n", 1, argv[2]);

    useek(DISK, offset);
    uread(DISK, buf, 12);
    uprintf("Read from disk after write: %s\n", 1, buf);
    return 0;
}



