#ifndef TSS_H_
#define TSS_H_

// Task state segment format
struct taskstate {
  uint32 link;         // Old ts selector
  uint32 esp0;         // Stack pointers and segment selectors
  uint16 ss0;        //   after an increase in privilege level
  uint16 padding1;
  uint32 *esp1;
  uint16 ss1;
  uint16 padding2;
  uint32 *esp2;
  uint16 ss2;
  uint16 padding3;
  void *cr3;         // Page directory base
  uint32 *eip;         // Saved state from last task switch
  uint32 eflags;
  uint32 eax;          // More saved state (registers)
  uint32 ecx;
  uint32 edx;
  uint32 ebx;
  uint32 *esp;
  uint32 *ebp;
  uint32 esi;
  uint32 edi;
  uint16 es;         // Even more saved state (segment selectors)
  uint16 padding4;
  uint16 cs;
  uint16 padding5;
  uint16 ss;
  uint16 padding6;
  uint16 ds;
  uint16 padding7;
  uint16 fs;
  uint16 padding8;
  uint16 gs;
  uint16 padding9;
  uint16 ldt;
  uint16 padding10;
  uint16 t;          // Trap on task switch
  uint16 iomb;       // I/O map base address
};


#endif
