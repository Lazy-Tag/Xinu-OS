#include "varlen.h"

/* in file Lab4_getheap.c */
extern char *getheap(uint32);

/* in file Lab4_log2ph.c */
extern uint32 log2ph(char *);

/* in file Lab4_alloc_free.c */
extern uint32 palloc();
extern syscall pfree(uint32);

/* in file Lab4_fillpgentry.c */
extern int isfree(char *);
extern uint32 fill_pgentry(char *, uint32, uint32, int);

/*in file Lab4_addargs.S */
extern status u2021201780_addargs(pid32, int32, int32*, int32, char*, void*);

/* in file Lab4_invlpg.c */
extern void invlpg(void *);