#include <xinu.h>

/* private variables */
static char *mem_start_brk;  /* points to first byte of heap */
static char *mem_brk;        /* points to last byte of heap */
static char *mem_max_addr;   /* largest legal heap address */

void mem_init(void);

void mem_deinit(void);

void *mem_sbrk(int incr);

void mem_reset_brk(void);

void *mem_heap_lo(void);

void *mem_heap_hi(void);

uint32 mem_heapsize(void);
/* 
 * mem_init - initialize the memory system model
 */
void mem_init(void) {
    intmask mask;            /* Saved interrupt mask		*/

    mask = disable();

    kprintf("Heap initialize\n");
    /* allocate the storage we will use to model the available VM */
    if ((uint32)(mem_start_brk = (char *) getmem(MAX_HEAP)) == SYSERR) {
        panic("mem_init_vm: malloc error\n");
    }

    mem_max_addr = mem_start_brk + MAX_HEAP;  /* max legal heap address */
    mem_brk = mem_start_brk;                  /* heap is empty initially */
    mm_init();

    restore(mask);
}

/* 
 * mem_deinit - free the storage used by the memory system model
 */
void mem_deinit(void) {
    freemem(mem_start_brk, MAX_HEAP);
}

/*
 * mem_reset_brk - reset the simulated brk pointer to make an empty heap
 */
void mem_reset_brk() {
    mem_brk = mem_start_brk;
}

/* 
 * mem_sbrk - simple model of the sbrk function. Extends the heap 
 *    by incr bytes and returns the start address of the new area. In
 *    this model, the heap cannot be shrunk.
 */
void *mem_sbrk(int incr) {
    char *old_brk = mem_brk;

    if ((incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        panic("ERROR: mem_sbrk failed. Ran out of memory...\n");
    }
    mem_brk += incr;
    return (void *) old_brk;
}

/*
 * mem_heap_lo - return address of the first heap byte
 */
void *mem_heap_lo() {
    return (void *) mem_start_brk;
}

/* 
 * mem_heap_hi - return address of last heap byte
 */
void *mem_heap_hi() {
    return (void *) (mem_brk - 1);
}

/*
 * mem_heapsize() - returns the heap size in bytes
 */
uint32 mem_heapsize() {
    return (uint32)(mem_brk - mem_start_brk);
}