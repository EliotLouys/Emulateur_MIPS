
/**
 * @file types.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Oct 31 19:09:31 2013
 * @brief Types. 
 *
 * Types.
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

  /** Unsigned ints (shorthand) and words. */
  typedef uint32_t uint, word;

  /** Bytes. */
  typedef uint8_t  byte;

  /** 32-bit address. */
  typedef uint32_t vaddr32;

  /** 64-bit address. */
  typedef uint64_t vaddr64;

  /** Addresses and virtual addresses. */
  typedef union {
    vaddr32 _32; /**< 32-bit (possibly virtual) address */
    vaddr64 _64; /**< 64-bit (possibly virtual) address */
  } vaddr, addr;

  /** Static strings. */
  typedef char string[ MAX_STR + 1 ];

  /** Endianness. */
  typedef enum {
    UNDEF,  /**< Undefined. */
    LSB,    /**< Least Significant Byte first. */
    MSB     /**< Most Significant Byte first. */
  } endianness;

  /** Structure for assembler instructions */
  struct R{
    unsigned int func:6,
    sa:5,
    rd:5,
    rt:5,
    rs:5,
    opcode:6;
  };

  struct I{
    unsigned int immediate:16,//ou offset
    rt:5,
    rs:5,
    opcode:6;
  };

  struct J{
    unsigned int target:26,
    opcode:6;
  };
  
  union instruct_poly {
    uint32_t asInt;
    struct R asR;
    struct I asI;
    struct J asJ;
    unsigned char asCharArray[4];
  };

  typedef struct {
    char       nom[17];
    char       type[3]; //R=1, I=2, J=3
    int       nb_param;
    char      ordre[4][17];
    uint32_t  signature;
    uint32_t  masque;
  } instruct_def;

/*
  typedef struct {
    int nb_point;
    uint32_t addr_break[100];
  } break_point;*/

  typedef struct _link{
    uint32_t addr_break;
    struct _link *next;
  } *break_point;
  
  
  
#ifdef __cplusplus
}
#endif

#endif /* _TYPES_H_ */
