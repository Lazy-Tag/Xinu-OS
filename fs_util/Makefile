all: fs.img

FS_FILES = test \
		   test.c \
		   asm.S  \
		   test.o \
		   asm.o

fs.img: mkfs $(FS_FILES)
	./mkfs fs.img $(FS_FILES)

mkfs: mkfs.c
	$(CC) -o mkfs mkfs.c

%.o: %.c
	$(CC) -c -m32 -o $@ $^

%.o: %.S
	$(CC) -c -m32 -o $@ $^

test: test.o asm.o
	$(LD) -m elf_i386 -e main -Ttext 0x80001000 -o $@ $^

clean:
	rm -f mkfs fs.img *.o test 
