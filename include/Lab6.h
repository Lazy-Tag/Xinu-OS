#include <elf.h>

#define PHNUM 10

struct	diskcblk {
	uint32	lba_addr;
    uint16 cache[256];
    uint32	prev_sector;
    bool8 is_dirty;
};

extern	struct	diskcblk	disktab[];

typedef struct {
	int offset;
	int size;
	char   name[128];
}File;

#define ELF_PROG_LOAD 1

extern void diskinit();

extern devcall diskread(struct dentry *, char *, int32);

extern devcall diskgetc(struct dentry *);

extern devcall diskseek(struct dentry *, uint32);

extern devcall diskwrite(struct dentry *, char *, int32);

extern devcall diskputc(struct dentry *, char);

extern devcall diskflush(struct dentry *);

extern	syscall	u2021201780_seek(did32, uint32);

extern  uint32 k2021201780_load_code(uint32, uint32);

