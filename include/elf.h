// Format of an ELF executable file

#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

// File header
typedef  struct {
  uint32 magic;  // must equal ELF_MAGIC
  char elf[12];
  uint16 type;
  uint16 machine;
  uint32 version;
  uint32 entry;
  uint32 phoff;
  uint32 shoff;
  uint32 flags;
  uint16 ehsize;
  uint16 phentsize;
  uint16 phnum;
  uint16 shentsize;
  uint16 shnum;
  uint16 shstrndx;
}elfhdr;

// Program section header
typedef  struct {
  uint32 type;
  uint32 off;
  uint32 vaddr;
  uint32 paddr;
  uint32 filesz;
  uint32 memsz;
  uint32 flags;
  uint32 align;
}proghdr;

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4
