
/**
 * @file relocator.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 20:11:00 2013
 * @brief MIPS 32 bits relocation.
 *
 * MIPS 32 bits relocation.
 */

#ifndef _RELOCATOR_H_
#define _RELOCATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

  /** MIPS reloc type: None. */
#define R_MIPS_NONE      0
  /** MIPS reloc type: 16-bit. */
#define R_MIPS_16        1
  /** MIPS reloc type: 32-bit. */
#define R_MIPS_32        2
  /** MIPS reloc type: 32-bit, relative. */
#define R_MIPS_REL32     3
  /** MIPS reloc type: 26-bit. */
#define R_MIPS_26        4
  /** MIPS reloc type: HI16. */
#define R_MIPS_HI16      5
  /** MIPS reloc type: LO16. */
#define R_MIPS_LO16      6
  /** MIPS reloc type: 16-bit general purpose register. */
#define R_MIPS_GPREL16   7
  /** MIPS reloc type: Literal. */
#define R_MIPS_LITERAL   8
  /** MIPS reloc type: Global offset table, 16 bits. */
#define R_MIPS_GOT16     9
  /** MIPS reloc type: Program counter, 16 bits. */
#define R_MIPS_PC16     10
  /** MIPS reloc type: CALL16. */
#define R_MIPS_CALL16   11
  /** MIPS reloc type: 32-bit general purpose register. */
#define R_MIPS_GPREL32  12
  /** MIPS reloc type: Global offset table, HI16. */
#define R_MIPS_GOTHI16  21
  /** MIPS reloc type: Global offset table, LO16. */
#define R_MIPS_GOTLO16  22
  /** MIPS reloc type: CALL to HI16. */
#define R_MIPS_CALLHI16 30
  /** MIPS reloc type: CALL to LO16. */
#define R_MIPS_CALLLO16 31

  /** MIPS relocation names. */
static char *MIPS32_REL[] = {
    "R_MIPS_NONE", "R_MIPS_16", "R_MIPS_32", "R_MIPS_REL32",
    "R_MIPS_26", "R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16",
    "R_MIPS_LITERAL", "R_MIPS_GOT16", "R_MIPS_PC16", "R_MIPS_CALL16",
    "R_MIPS_GPREL32", "", "", "", "", "", "", "", "", "R_MIPS_GOTHI16",
    "R_MIPS_GOTLO16", "", "", "", "", "", "", "", "R_MIPS_CALLHI16",
    "R_MIPS_CALLLO16"
};

#ifdef __cplusplus
}
#endif

#endif /* _RELOCATOR_H_ */
