/* in file Lab4_getheap.c */
extern char *getheap(uint32);

/* in file Lab4_log2ph.c */
extern uint32 log2ph(char *);

/* in file Lab4_alloc_free.c */
extern uint32 physical_addr_alloc();
extern syscall physical_addr_free(uint32);

/* in file Lab4_fillpgentry.c */
extern int isfree(char *);
extern uint32 fill_page_entry(char *, uint32, uint32, int);

/*in file Lab4_addargs.S */
extern status u2021201780_addargs(pid32, int32, int32*, int32, char*, void*);

/* in file Lab4_invlpg.c */
extern void invlpg(void *);

/* in file Lab4_umalloc_free.c */
extern void* u2021201780_malloc(uint32);

extern void u2021201780_free(void*);

extern void u2021201780_heap_init();

/* in file Lab4_do_stk_expand.c */
extern void _do_stk_expand(uint32);


/* in file Lab4_memlib.c */
extern void mem_init(void);

extern void mem_deinit(void);

extern void *mem_sbrk(int incr);

extern void mem_reset_brk(void);

extern void *mem_heap_lo(void);

extern void *mem_heap_hi(void);

extern uint32 mem_heapsize(void);

/* in file Lab4_memory_pool.c */
extern int mm_init (void);

extern void *mm_malloc (uint32 size);

extern void mm_free(void *ptr);

extern void *mm_realloc(void *ptr, uint32 size);