
/**
 * @file syms.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 13:46:17 2013
 * @brief Binary symbols.
 *
 * Binary symbols.
 */

#ifndef _SYMS_H_
#define _SYMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

  /** Symbol types in emulator. */
  typedef enum {
    notype,   /**< Undefined. */
    object,   /**< Object. */
    function, /**< Function. */
    section,  /**< Section. */
    file,     /**< File. */
    linked    /**< Link. */
  } sym_type;

  /** Symbol in emulator. */
  typedef struct {
    char      *name;      /**< Name. */
    vaddr      addr;      /**< Address. */
    uint       size;      /**< Size (in bytes). */
    sym_type   type;      /**< Type. */
    uint     scnidx;      /**< Section index. */
    uint     scnidx_link; /**< Section index link. */
  } sym;

  /** Symbol table in emulator. */
  typedef struct {
    uint       size; /**< Number of symbol in symbol table.. */
    sym       *sym;  /**< Address of first symbol. */
  } stab;

sym  new_sym32( char *name, vaddr32 addr, uint size, sym_type type, uint scnidx );
sym  new_sym64( char *name, vaddr64 addr, uint size, sym_type type, uint scnidx );
void sym32_print( sym s );
void sym64_print( sym s );

stab new_stab( uint size );
void stab32_print( stab s );
void stab64_print( stab s );
void del_stab( stab s );

#ifdef __cplusplus
}
#endif

#endif /* _SYMS_H_ */
