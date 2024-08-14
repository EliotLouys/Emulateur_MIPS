
/**
 * @file elf.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Oct 31 23:18:01 2013
 * @brief ELF binary format.
 *
 * ELF binary format.
 */

#ifndef _ELF_H_
#define _ELF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#include "types.h"
#include "elf/syms.h"
#include "elf/section.h"
#include "elf/relocator.h"

  /** ELF32: 32-bit addresses. */
typedef uint32_t Elf32_Addr;
  /** ELF32: 32-bit offsets. */
typedef uint32_t Elf32_Off;
  /** ELF32: Unsigned half-words. */
typedef uint16_t Elf32_Half;
  /** ELF32: Signed words. */
typedef int32_t  Elf32_Sword;
  /** ELF32: Unsigned words. */
typedef uint32_t Elf32_Word;

  /** ELF64: 64-bit addresses. */
typedef uint64_t Elf64_Addr;
  /** ELF64: 64-bit offsets. */
typedef uint64_t Elf64_Off;
  /** ELF64: Unsigned half-words. */
typedef uint16_t Elf64_Half;
  /** ELF64: Unsigned words. */
typedef uint32_t Elf64_Word;
  /** ELF64: Signed words. */
typedef int32_t  Elf64_SWord;
  /** ELF64: Unsigned double words. */
typedef uint64_t Elf64_Xword;
  /** ELF64: Signed double words. */
typedef int64_t  Elf64_Sxword;

  /** Size of ELF identification block (in bytes). */
#define EI_NIDENT     16

  /** ELF32 header. */
  typedef struct {

    unsigned char e_ident[EI_NIDENT]; /**< ELF identification info. */
    Elf32_Half    e_type;             /**< File type. */
    Elf32_Half    e_machine;          /**< Target architecture. */
    Elf32_Word    e_version;          /**< ELF version. */
    Elf32_Addr    e_entry;            /**< If relevant, entry point address. */
    Elf32_Off     e_phoff;            /**< If relevant, offset to program header table. */
    Elf32_Off     e_shoff;            /**< If relevant, offset to section header table. */
    Elf32_Word    e_flags;            /**< Processor-specific flags. */
    Elf32_Half    e_ehsize;           /**< Size of ELF header (in bytes). */
    Elf32_Half    e_phentsize;        /**< Size of program header entry size (in bytes). */
    Elf32_Half    e_phnum;            /**< Number of entries in program header table. */
    Elf32_Half    e_shentsize;        /**< Size of entries in section header table. */
    Elf32_Half    e_shnum;            /**< Number of entries in section header table. */
    Elf32_Half    e_shstrndx;         /**< Index of section names table in section header table. */

  } Elf32_Ehdr;


  /** ELF64 header. */
  typedef struct {
  
    unsigned char e_ident[EI_NIDENT]; /**< ELF identification info. */ 
    Elf64_Half    e_type;             /**< File type. */
    Elf64_Half    e_machine;          /**< Target architecture. */
    Elf64_Word    e_version;          /**< ELF version. */
    Elf64_Addr    e_entry;            /**< If relevant, entry point address. */
    Elf64_Off     e_phoff;            /**< If relevant, offset to program header table. */
    Elf64_Off     e_shoff;            /**< If relevant, offset to section header table. */
    Elf64_Word    e_flags;            /**< Processor-specific flags. */
    Elf64_Half    e_ehsize;           /**< Size of ELF header (in bytes). */
    Elf64_Half    e_phentsize;        /**< Size of program header entry size (in bytes). */
    Elf64_Half    e_phnum;            /**< Number of entries in program header table. */
    Elf64_Half    e_shentsize;        /**< Size of entries in section header table. */
    Elf64_Half    e_shnum;            /**< Number of entries in section header table. */
    Elf64_Half    e_shstrndx;         /**< Index of section names table in section header table. */

  } Elf64_Ehdr;

  /** ELF file type: unknown type. */
#define ET_NONE    0
  /** ELF file type: relocatable file. */
#define ET_REL     1
  /** ELF file type: executable file. */
#define ET_EXEC    2
  /** ELF file type: shared object file. */
#define ET_DYN     3
  /** ELF file type: core dump file. */
#define ET_CORE    4
  /** ELF file type: processor specific. */
#define ET_LOPROC  0xff00
  /** ELF file type: processor specific. */
#define ET_HIPROC  0xffff

  /** ELF machine type: Unknown. */
#define EM_NONE    ET_NONE
  /** ELF machine type: AT&T WE32100. */
#define EM_M32     1
  /** ELF machine type: Sun Microsystems SPARC. */
#define EM_SPARC   2
  /** ELF machine type: Intel i386. */
#define EM_386     3
  /** ELF machine type: Motorola 68k. */
#define EM_68K     4
  /** ELF machine type: Motorola 88k. */
#define EM_88K     5
  /** ELF machine type: Intel i860. */
#define EM_860     7
  /** ELF machine type: MIPS RS3000, MSB. */
#define EM_MIPS    8

  /** ELF version: none. */
#define EV_NONE    0
  /** ELF version: current. */
#define EV_CURRENT 1

  /** ELF identitication block offset: first magic byte. */
#define EI_MAG0    0
  /** ELF identitication block offset: second magic byte. */
#define EI_MAG1    1
  /** ELF identitication block offset: third magic byte. */
#define EI_MAG2    2
  /** ELF identitication block offset: fourth magic byte. */
#define EI_MAG3    3
  /** ELF identitication block offset: class. */
#define EI_CLASS   4
  /** ELF identitication block offset: data storage. */
#define EI_DATA    5
  /** ELF identitication block offset: version. */
#define EI_VERSION 6
  /** ELF identitication block offset: padding bytes. */
#define EI_PAD     7
  /** ELF identitication block offset: @c NIDENT. */
#define EI_NIDENT  16

  /** ELF Magic, first byte. */
#define ELF_MAG0   0x7f
  /** ELF Magic, second byte. */
#define ELF_MAG1   'E'
  /** ELF Magic, third byte. */
#define ELF_MAG2   'L'
  /** ELF Magic, fourth byte. */
#define ELF_MAG3   'F'

  /** ELF Class: @ NONE . */
#define ELFCLASSNONE 0
  /** ELF Class: @ 32-bits . */
#define ELFCLASS32   1
  /** ELF Class: @ 64-bits . */
#define ELFCLASS64   2

  /** ELF Data storage: @ NONE . */
#define ELFDATANONE  0
  /** ELF Data storage: @ LSB . */
#define ELFDATA2LSB  1
  /** ELF Data storage: @ MSB . */
#define ELFDATA2MSB  2

  /** ELF Section Header, @c UNDEF . */
#define SHN_UNDEF     0
  /** ELF Section Header, @c LORESERVE . */
#define SHN_LORESERVE 0xff00
  /** ELF Section Header, @c LOPROC . */
#define SHN_LOPROC    0xff00
  /** ELF Section Header, @c HIPROC . */
#define SHN_HIPROC    0xff1f
  /** ELF Section Header, @c ABS . */
#define SHN_ABS       0xfff1
  /** ELF Section Header, @c COMMON . */
#define SHN_COMMON    0xfff2
  /** ELF Section Header, @c HIRESERVE . */
#define SHN_HIRESERVE 0xffff

  /** ELF32 Section header info. */
typedef struct {

    Elf32_Word   sh_name;       /**< Index of section name in section header string table. */
    Elf32_Word   sh_type;       /**< Section type. */
    Elf32_Word   sh_flags;      /**< Section attributes. */
    Elf32_Addr   sh_addr;       /**< If non-zero, address of first byte in memory. */
    Elf32_Off    sh_offset;     /**< Index of first byte in the ELF file. */
    Elf32_Word   sh_size;       /**< Section size (in bytes). */
    Elf32_Word   sh_link;       /**< Index in section header table. */
    Elf32_Word   sh_info;       /**< Additional info. */
    Elf32_Word   sh_addralign;  /**< Alignment constraint (zero or power of two). */
    Elf32_Word   sh_entsize;    /**< If relevant, size of section entries (symbols, relocs, etc.) */

} Elf32_Shdr;


  /** ELF64 Section header info. */
typedef struct {

    Elf64_Word   sh_name;       /**< Index of section in section header string table. */ 
    Elf64_Word   sh_type;       /**< Section type. */
    Elf64_Xword  sh_flags;      /**< Section attributes. */
    Elf64_Addr   sh_addr;       /**< If non-zero, address of first byte in memory. */
    Elf64_Off    sh_offset;     /**< Index of first byte in ELF file. */
    Elf64_Xword  sh_size;       /**< Section size (in bytes). */
    Elf64_Word   sh_link;       /**< Index in section header table. */
    Elf64_Word   sh_info;       /**< Additional info. */
    Elf64_Xword  sh_addralign;  /**< Alignament constraint (zero or power of two). */
    Elf64_Xword  sh_entsize;    /**< If relevant, size of section entries (symbols, relocs, etc.) */

} Elf64_Shdr;

  /** ELF Section type: inactive. */
#define SHT_NULL       0
  /** ELF Section type: program defined. */
#define SHT_PROGBITS   1
  /** ELF Section type: symbol table. */
#define SHT_SYMTAB     2
  /** ELF Section type: string table. */
#define SHT_STRTAB     3
  /** ELF Section type: relocation info with addend. */
#define SHT_RELA       4
  /** ELF Section type: symbol hash table. */
#define SHT_HASH       5
  /** ELF Section type: dynamic linking info. */
#define SHT_DYNAMIC    6
  /** ELF Section type: markup info. */
#define SHT_NOTE       7
  /** ELF Section type: empy progbits section. */
#define SHT_NOBITS     8
  /** ELF Section type: relocation info without addend. */
#define SHT_REL        9
  /** ELF Section type: undefined/unused. */
#define SHT_SHLIB      10
  /** ELF Section type: dynamic loading info. */
#define SHT_DYNSYM     11
  /** ELF Section type: processor specific. */
#define SHT_LOPROC     0x70000000
  /** ELF Section type: processor specific. */
#define SHT_HIPROC     0x7fffffff
  /** ELF Section type: application specific. */
#define SHT_LOUSER     0x80000000
  /** ELF Section type: application specific. */
#define SHT_HIUSER     0xffffffff

  /** ELF Section attribute: section contains writable data. */
#define SHF_WRITE      0x1
  /** ELF Section attribute: section used during program execution. */
#define SHF_ALLOC      0x2
  /** ELF Section attribute: section contains executable code. */
#define SHF_EXECINSTR  0x4
  /** ELF Section attribute: processor specific. */
#define SHF_MASKPROC   0xf0000000

  /** First symbol table entry (reserved). */
#define STN_UNDEF      0

  /** ELF32 symbols. */
  typedef struct {

    Elf32_Word      st_name;   /**< Symbol name offset in associated symbol table. */
    Elf32_Addr      st_value;  /**< Symbol value.  */
    Elf32_Word      st_size;   /**< Symbol size. */
    unsigned char   st_info;   /**< Symbol information. */
    unsigned char   st_other;  /**< Symbol additional information. */
    Elf32_Half      st_shndx;  /**< Symbol section header index. */
    
  } Elf32_Sym;
  
  /** ELF64 symbols. */
  typedef struct {
    
    Elf64_Word     st_name;    /**< Symbol name offset in associated symbol table. */
    unsigned char  st_info;    /**< Symbol information. */
    unsigned char  st_other;   /**< Symbol additional information. */
    Elf64_Half     st_shndx;   /**< Symbol section header index. */
    Elf64_Addr     st_value;   /**< Symbol value. */
    Elf64_Xword    st_size;    /**< Symbol size. */
    
  } Elf64_Sym;

  /**
   * @brief Get bind information from @c st_info value.
   * @param i @c st_info field.
   */
#define ELF32_ST_BIND(i)    ((i)>>4)
  
  /**
   * @brief Get type information from @c st_info value.
   * @param i @c st_info field. 
   */
#define ELF32_ST_TYPE(i)    ((i)&0xf)

  /**
   * @brief Produce @c st_info value from bind and type informations.
   * @param b Bind information. 
   * @param t Type information. 
   */
#define ELF32_ST_INFO(b,t)  (((b)<<4)+((t)&0xf))

  /** ELF Symbol attributes: local symbol. */
#define STB_LOCAL       0
  /** ELF Symbol attributes: global symbol. */
#define STB_GLOBAL      1
  /** ELF Symbol attributes: like global symbols, but superseded by globals. */
#define STB_WEAK        2
  /** ELF Symbol attributes: processor specific. */
#define STB_LOPROC      13
  /** ELF Symbol attributes: processor specific. */
#define STB_HIPROC      15

  /** ELF Symbol type: undefined symbol type. */
#define STT_NOTYPE      0
  /** ELF Symbol type: symbol relates to an object. */
#define STT_OBJECT      1
  /** ELF Symbol type: symbol relates to a function or otherwise executable object. */
#define STT_FUNC        2
  /** ELF Symbol type: symbol relates to a section. */
#define STT_SECTION     3
  /** ELF Symbol type: symbol is the name of the associated source file. */
#define STT_FILE        4
  /** ELF Symbol type: processor specific. */
#define STT_LOPROC      13
  /** ELF Symbol type: processor specific. */
#define STT_HIPROC      15


  /** ELF32 relocation entry, without addend. */
  typedef struct {

    Elf32_Addr   r_offset; /**< Reloc offset. */
    Elf32_Word   r_info;   /**< Reloc info. */

  } Elf32_Rel;

  /** ELF32 relocation entry, with addend. */
  typedef struct {

    Elf32_Addr   r_offset; /**< Reloc offset. */
    Elf32_Word   r_info;   /**< Reloc info. */
    Elf32_Sword  r_addend; /**< Reloc addend. */

  } Elf32_Rela;

  /** ELF64 relocation entry, without addend. */
  typedef struct {

    Elf64_Addr  r_offset; /**< Reloc offset. */
    Elf64_Xword r_info;   /**< Reloc info. */

  } Elf64_Rel;

  /** ELF64 relocation entry, with addend. */
  typedef struct {

    Elf64_Addr    r_offset; /**< Reloc offset. */
    Elf64_Xword   r_info;   /**< Reloc info. */
    Elf64_Sxword  r_addend; /**< Reloc addend. */

  } Elf64_Rela;

  /**
   * @brief ELF32 reloc: Get relocated symbol from reloc info. 
   * @param i ELF32 reloc info. 
   */
#define ELF32_R_SYM(i)      ((i)>>8)

  /**
   * @brief ELF32 reloc: Get relocation type from reloc info. 
   * @param i ELF32 reloc info. 
   */
#define ELF32_R_TYPE(i)     ((unsigned char)(i))

  /**
   * @brief ELF32 reloc: Produce relocation info from symbol and reloc type info. 
   * @param s relocated symbol. 
   * @param t relocation type. 
   */
#define ELF32_R_INFO(s,t)   (((s)<<8)+(unsigned char)(t))


  /**
   * @brief ELF64 reloc: Get relocated symbol from reloc info. 
   * @param i ELF64 reloc info. 
   */
#define ELF64_R_SYM(i)      ((i)>>32)

  /**
   * @brief ELF64 reloc: Get relocation type from reloc info. 
   * @param i ELF64 reloc info. 
   */
#define ELF64_R_TYPE(i)     ((i)&0xffffffffL)

  /**
   * @brief ELF64 reloc: Produce relocation info from symbol and reloc type info. 
   * @param s relocated symbol. 
   * @param t relocation type. 
   */
#define ELF64_R_INFO(s,t)   (((s)<<32)+((t)&0xffffffffL))

  typedef struct {

    Elf32_Word      p_type;
    Elf32_Off       p_offset;
    Elf32_Addr      p_vaddr;
    Elf32_Addr      p_paddr;
    Elf32_Word      p_filesz;
    Elf32_Word      p_memsz;
    Elf32_Word      p_flags;
    Elf32_Word      p_align;

  } Elf32_Phdr;

  typedef struct {

    Elf64_Word     p_type;
    Elf64_Word     p_flags;
    Elf64_Off      p_offset;
    Elf64_Addr     p_vaddr;
    Elf64_Addr     p_paddr;
    Elf64_Xword    p_filesz;
    Elf64_Xword    p_memsz;
    Elf64_Xword    p_align;

  } Elf64_Phdr;

#define PT_NULL      0
#define PT_LOAD      1
#define PT_DYNAMIC   2
#define PT_INTERP    3
#define PT_NOTE      4
#define PT_SHLIB     5
#define PT_PHDR      6
#define PT_LOPROC    0x70000000
#define PT_HIPROC    0x7fffffff



  /**
   * @brief Get endianness from ELF header. 
   * @param ehdr ELF header. 
   */
#define ENDIANNESS_FROM_EHDR(ehdr) ((unsigned char*)ehdr)[EI_DATA]

  /**
   * @brief Get machine width from ELF header. 
   * @param ehdr ELF header. 
   */
#define WIDTH_FROM_EHDR(ehdr)      ((unsigned char*)ehdr)[EI_CLASS]==ELFCLASS32?32:64


  byte *__Elf_Rel_flip_endianness( byte * rel, uint width, endianness e );

  int     assert_elf_file( FILE *fp );
  byte *__elf_get_ehdr( FILE * fp );
  int     elf_get_arch_info( FILE *fp, uint *machine, endianness *endianness, uint *width );
  byte   *elf_extract_scn_by_name( byte *ehdr, FILE *fp, char *name, uint *secsz, uint *file_offset, int *scnidx );
  byte   *elf_extract_string_table( byte *ehdr, FILE *fp, char *name, uint *sz );
  byte   *elf_extract_section_names( FILE *fp, uint *size );
  void    print_string_table( byte *stab, unsigned int sz );
  byte   *elf_extract_symbol_table( byte *ehdr, FILE *fp, char *name, unsigned int *nsymbol );
  
  byte   *elf_extract_section_header_table( byte * ehdr, FILE * fp );
  int     elf_load_scntab( FILE *fp, unsigned int width, scntab *scntab );
  int     elf_load_symtab( FILE *fp,  unsigned int width, unsigned int endian,  stab *symtab );

#ifdef __cplusplus
}
#endif

#endif /* _ELF_H_ */
