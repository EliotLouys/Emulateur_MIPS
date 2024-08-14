/**
 * @file section.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 14:54:23 2013
 * @brief Section handling.
 *
 * Section handling.
 */

#ifndef _SECTION_H_
#define _SECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

  /** Section info in emulator. */
  typedef struct {
    vaddr   addr; /**< Address of first byte in memory. */
    char   *name; /**< Name of section. */
  } scn;

  /** Table of sections in emulator. */
  typedef struct {
    uint   size;  /**< Number of sections. */
    scn   *scn;   /**< Address of first section. */
  } scntab;

scntab new_scntab( uint size );
void   del_scntab( scntab scntab );
void   print_scntab( scntab scntab );

#ifdef __cplusplus
}
#endif

#endif /* _SECTION_H_ */
