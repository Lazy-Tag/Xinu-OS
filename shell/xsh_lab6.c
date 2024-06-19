/* xsh_lab5.c - xsh_test_disk */

#include <xinu.h>

shellcmd xsh_lab6_ls(int nargs, char *args[]) {
    u2021201780_seek(DISK, 0);
    File dir;
    u2021201780_read(DISK, (char*)&dir, sizeof(File));

    for (int i = 0; i < dir.size; i++) {
        File file;
        u2021201780_read(DISK, (char*)&file, sizeof(File));
        u2021201780_printf("filename: %s offset: %d\n", 2, file.name, file.offset);
    }

    return 0;
}

shellcmd xsh_lab6_exec(int nargs, char *args[]) {
    u2021201780_seek(DISK, 0);
    if (nargs < 2) {
        u2021201780_printf("Usage: exec <filename>\n");
        return 1;
    }
    char* file_name = args[1];
    File dir, file;
    u2021201780_read(DISK, (char*)&dir, sizeof(File));
    bool8 flag = FALSE;
    uint32 offset;
    for (int i = 0; i < dir.size; i++) {
        u2021201780_read(DISK, (char*)&file, sizeof(File));
        if (strcmp(file_name, file.name) == 0) {
            offset = file.offset;
            u2021201780_seek(DISK, file.offset);
            flag = TRUE;
            break;
        }
    }

    if (!flag) {
        u2021201780_printf("File not found\n", 0);
        return SYSERR;
    }

    elfhdr elf;
    u2021201780_read(DISK, (char*)&elf, sizeof(elfhdr));

    uint32 start_addr;
    for (int i = 1; i < elf.phnum; i ++ ) {
        proghdr ph;
        uint32 now_off = offset + elf.phoff + i * elf.phentsize;
        u2021201780_seek(DISK, now_off);
        u2021201780_read(DISK, (char*)&ph, sizeof(proghdr));
        if (ph.type == ELF_PROG_LOAD) {
            uint32 memsz = ph.memsz;
            uint32 paddr = ph.paddr & ~(0x80000000);
            int32 padding = 0;
            if (paddr == 0x4000) {
                padding = 1;
                paddr = 0x3000;
            }
            uint32 addr = u2021201780_load_code((offset + paddr)
                    | (padding << 31), memsz);
            if (i == 1) {
                start_addr = (addr >> 12) << 12;
            }
        }
    }

    int32 tlen = 0, tmparg, ntok = 0;
    char tokbuf[SHELL_BUFLEN + SHELL_MAXTOK];
    memset(tokbuf, 0, SHELL_BUFLEN + SHELL_MAXTOK);
    int32 tok[SHELL_MAXTOK];
    for (int i = 1; i < nargs; i++) {
        tok[i - 1] = tlen;
        int32 len = strlen(args[i]);
        memcpy(tokbuf + tlen, args[i], len);
        tlen += len + 1;
        ntok ++ ;
    }

    pid32 child = u2021201780_create(start_addr, SHELL_CMDSTK,
                       SHELL_CMDPRIO, args[1], 2, ntok, &tmparg);

    if ((child == SYSERR) ||
            (u2021201780_addargs(child, ntok, tok, tlen, tokbuf, &tmparg)
             == SYSERR)) {
        u2021201780_printf("Error: %s cannot be created\n", 1, args[0]);
        return SYSERR;
    }

    u2021201780_resume(child);

    return 0;
}

shellcmd xsh_lab6_fcat(int nargs, char *args[]) {
    u2021201780_seek(DISK, 0);
    if (nargs < 2) {
        u2021201780_printf("Usage: cat <filename>\n");
        return 1;
    }
    char* file_name = args[1];
    File dir, file;
    u2021201780_read(DISK, (char*)&dir, sizeof(File));
    bool8 flag = FALSE;
    for (int i = 0; i < dir.size; i++) {
        u2021201780_read(DISK, (char*)&file, sizeof(File));
        if (strcmp(file_name, file.name) == 0) {
            u2021201780_seek(DISK, file.offset);
            flag = TRUE;
            break;
        }
    }

    if (!flag) {
        u2021201780_printf("File not found\n", 0);
        return SYSERR;
    }

    char buf[128];
    u2021201780_read(DISK, buf, 128);
    u2021201780_printf("%s\n", 1, buf);

    return 0;
}

uint32 k2021201780_load_code(uint32 offset, uint32 size) {
    int32 padding = (offset & 0x80000000);
    if (padding) {
        offset &= ~(0x80000000);
        getmem(PAGE_SIZE);
    }
    uint32 addr = getmem(size);
    u2021201780_seek(DISK, offset);
    u2021201780_read(DISK, (char*)addr, size);

    return addr;
}