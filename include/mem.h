
/**
 * @file mem.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Nov  1 00:56:03 2013
 * @brief Memory.
 *
 * Memory.
 */

#ifndef _MEM_H_
#define _MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "types.h"

  /** Size. */
  typedef union {
    uint32_t _32;       /**< 32-bit size. */
    uint64_t _64;       /**< 64-bit size. */
  } vsize;

  /** Memory segment description. */
  typedef struct {
    char      *name;    /**< Name. */
    byte      *content; /**< Contents (raw bytes). */
    vaddr      start;   /**< Virtual start address. */
    vsize      size;    /**< Virtual size. */
    uint32_t   attr;    /**< Attributes. */
    int        scnidx;  /**< ELF section ID (because we are nice people). */
  } segment;

  /** Virtual memory (set of @ref segment ). */
  typedef struct {
    vsize       extent; /**< Total memory size. */
    uint32_t    nseg;   /**< Number of memory segments. */
    segment    *seg;    /**< Address of first segment */
  } *mem;

  /** Segment attribute: Read-only. */
#define R__   1
  /** Segment attribute: Read/Write. */
#define RW_   2
  /** Segment attribute: Read-eXecute. */
#define R_X   3
  /** Segment attribute: Undefined (e.g., @c .rodata segment). */
#define UNDEF 7 /* For optional sections (.rodata) */

  /** Memory page size (in KBytes). */
#define PGSZ          4  /* Page size:  4K */
  /** Stack size (in MBytes). */
#define STACKSZ       8  /* Stack size: 8M */
  /**  */
#define STACKSZ_BYTES (STACKSZ*1024*1024)

  /**
   * @brief Returns number of pages required to store given amount of bytes. 
   * @param b Number of bytes. 
   */
#define NPAGES(b) ( (b) ? ( (b)>>10 ) ? ((b)>>10)/PGSZ : 1 : 0 )


  /** Segment type: @c .rodata . */
#define RODATA      0
  /** Segment type: @c .text . */
#define TEXT        1
  /** Segment type: @c .data . */
#define DATA        2
  /** Segment type: @c .bss . */
#define BSS         3
  /** Segment type: @c .heap . */
#define HEAP        4
  /** Segment type: @c .libc.text . */
#define LIBC_TEXT   5
  /** Segment type: @c .libc.data . */
#define LIBC_DATA   6
  /** Segment type: @c .stack . */
#define STACK       7
  /** Segment type: @c .vsyscall . */
#define VSYSCALL    8 /* CAUTION: Make sure this one is the very last */

  /** Last segment index. */
#define LAST_SEG    ((VSYSCALL)+1)

  /**
   * @brief Return attributes from width and rights informations. 
   * @param width Width. 
   * @param rights Rights. 
   */
#define SCN_ATTR(width, rights)  (((width==0x1?32:64)<<8)+rights)

  /**
   * @brief Get rights from attributes. 
   * @param attr Attributes. 
   */
#define SCN_RIGHTS(attr)         (((attr)&0x000000ff))

  /**
   * @brief Get machine width from attributes. 
   * @param attr Attributes. 
   */
#define SCN_WIDTH(attr)          (((attr))>>8)

  mem    init_mem( uint32_t nseg );
  int    attach_scn_to_mem( mem vm, char *name, uint32_t attr );
  int    fill_mem_scn( mem vm, char *name, vsize sz, vaddr start, byte *content, int scnidx );
  void   print_mem( mem vm );
  void   del_mem( mem vm );

#ifdef __cplusplus
}
#endif

#endif /* _MEM_H_ */
