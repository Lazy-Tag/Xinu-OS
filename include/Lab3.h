#include <tss.h>

#define TI_GDT 0
#define KENRAL_LEVEL 0
#define USER_LEVEL 3
#define GDT_CODE 4
#define GDT_DATA 5
#define GDT_STACK 6
#define BASE_KENRAL_CS ((1<<3) + (TI_GDT<<2) + KENRAL_LEVEL)
#define BASE_KENRAL_DS ((2<<3) + (TI_GDT<<2) + KENRAL_LEVEL)
#define BASE_KENRAL_SS ((3<<3) + (TI_GDT<<2) + KENRAL_LEVEL)
#define BASE_USER_CS ((GDT_CODE<<3) + (TI_GDT<<2) + USER_LEVEL)
#define BASE_USER_DS ((GDT_DATA<<3) + (TI_GDT<<2) + USER_LEVEL)
#define BASE_USER_SS ((GDT_STACK<<3) + (TI_GDT<<2) + USER_LEVEL)
#define EFLAGS_MBS    (1 << 1)
#define EFLAGS_IF_0   (0 << 9)
#define EFLAGS_IF_1   (1 << 9)
#define EFLAGS_IOPL_0 (0 << 12)
#define EFLAGS_IOPL_3 (3 << 12)
#define GDT_TSS 7
#define history_NID 48

/* in file Lab3_ret_k2u.S */
extern	void	k2021201780_ret_k2u(void);

/* in file Lab3_ltss.S */
extern	void	k2021201780_ltss(int);

/* in file Lab3_ret_u2k.S */
extern	void	u2021201780_ret_u2k(void);

/* in file Lab3_printf.S */
extern	void	u2021201780_printf(char*, ...);

/* in file Lab3_sleep.S */
extern	void	u2021201780_sleep(int);

/* in file Lab3_sleepms.S */
extern	void	u2021201780_sleepms(int);

/* in file Lab3_fprintf.S */
extern	void	u2021201780_fprintf(did32, char*, ...);

/* in file Lab3_create.S */
extern	int	    u2021201780_create(void * , uint32, int, char *, uint32, ...);

/* in file Lab3_read.S */
extern	int	    u2021201780_read(did32, char*, int);

/* in file Lab3_resume.S */
extern	void	u2021201780_resume(pid32);

/* in file Lab3_receive.S */
extern	umsg32	u2021201780_receive(void);

/* in file Lab3_recvclr.S */
extern	umsg32	u2021201780_recvclr(void);

struct taskstate TSS;