/*
 * asm.h - Assembler help routines
 *
 * Copyright (c) 2001 EmuTOS development team
 *
 * Authors:
 *  LVL   Laurent Vogel
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */

/*
 * This file contains utility routines (macros) to 
 * manipulate special registers from C language.
 * 
 * available macros:
 *
 * WORD set_sr(WORD new); 
 *   sets sr to the new value, and return the old sr value 
 * WORD get_sr(void);
 *   returns the current value of sr. the CCR bits are not meaningful.
 * void stop2300(void);
 * void stop2500(void);
 *   the STOP immediate instruction
 * void regsafe_call(void *addr);
 *   Do a subroutine call with saving/restoring the CPU registers
 *
 * For clarity, please add such two lines above when adding 
 * new macros below.
 */

#ifndef ASM_H
#define ASM_H



extern void swp68w(int *);
extern void swp68l(long *);

#define swpw(x) swp68w(&x)
#define swpl(x) swp68l(&x)

/* OS entry points implemented in util/miscasm.S */
extern long trap1(int, ...);
extern long trap1_pexec(short mode, char* p1, void* p2, char* p3);


/*
 * WORD set_sr(WORD new); 
 *   sets sr to the new value, and return the old sr value 
 */

#define set_sr(a)                           \
__extension__                               \
({register short retvalue __asm__("d0");    \
  short _a = (short)(a);                    \
  __asm__ volatile                          \
  ("move.w sr,d0;                           \
    move.w %1,sr "                          \
  : "=r"(retvalue)   /* outputs */          \
  : "d"(_a)          /* inputs  */          \
  : "d0"             /* clobbered regs */   \
  );                                        \
  retvalue;                                 \
})

/*
 * WORD get_sr(void); 
 *   returns the current value of sr. 
 */

#define get_sr()                            \
__extension__                               \
({register short retvalue __asm__("d0");    \
  __asm__ volatile                          \
  ("move.w sr,d0 "                          \
  : "=r"(retvalue)   /* outputs */          \
  :                  /* inputs  */          \
  : "d0"             /* clobbered regs */   \
  );                                        \
  retvalue;                                 \
})



/*
 * LONG get_sp(void);
 *   returns the current value of sr. 
 */

#define get_sp()                            \
__extension__                               \
({register LONG retvalue __asm__("d0");    \
  __asm__ volatile                          \
  ("move.l sp,d0 "                          \
  : "=r"(retvalue)   /* outputs */          \
  :                  /* inputs  */          \
  : "d0"             /* clobbered regs */   \
  );                                        \
  retvalue;                                 \
})



/*
 * void ints_off(void);
 *   switches off interrupts, SR saved to stack
 */

#define ints_off()      \
__extension__           \
({__asm__ volatile      \
  ("move  sr, -(sp);    \
    ori.w #0x0700, sr " \
  );                    \
})



/*
 * void ints_on(void);
 *   switches interrupts on again, SR restored from stack
 */

#define ints_on()       \
__extension__           \
({__asm__ volatile      \
  ("move  (sp)+, sr "); \
})



/*
 * void stop2x00(void)
 *   the m68k STOP immediate instruction
 */

#define stop2300()                              \
__extension__                                   \
({__asm__ volatile                              \
  ("stop #0x2300 ");                            \
})

#define stop2500()                              \
__extension__                                   \
({__asm__ volatile                              \
  ("stop #0x2500 ");                            \
})



/*
 * void regsafe_call(void *addr)
 *   Saves all registers to the stack, calls the function
 *   that addr points to, and restores the registers afterwards.
 */
#define regsafe_call(addr)                          \
__extension__                                       \
({__asm__ volatile ("movem.l d0-d7/a0-a6,-(sp) ");  \
  ((void (*)(void))addr)();                         \
  __asm__ volatile ("movem.l (sp)+,d0-d7/a0-a6 ");  \
})


#endif /* ASM_H */
