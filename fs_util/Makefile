all: fs.img

CFLAGS = -m32 -fno-stack-protector

FS_FILES = ATest \
           ATest.c \
           asm.S  \
           ATest.o \
           asm.o \
           test \
           BTest \
           BTest.c \
           BTest.o


fs.img: mkfs $(FS_FILES)
	./mkfs fs.img $(FS_FILES)

mkfs: mkfs.c
	$(CC) -g -o mkfs mkfs.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $^

ATest: ATest.o asm.o
	$(LD) -m elf_i386 -e ATest -Ttext 0x80001000 -o $@ $^

BTest: BTest.o asm.o
	$(LD) -m elf_i386 -e BTest -Ttext 0x80001000 -o $@ $^

clean:
	rm -f mkfs fs.img *.o *Test
