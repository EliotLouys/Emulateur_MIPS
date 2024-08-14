
/**
 * @file cmd_load.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Aug 19 00:50:26 2020
 * @brief Emulator command: load.
 *
 * Emulator command: load.
 */

#include "commands.h"
#include "mem.h"
#include "bits.h"
#include "notify.h"
#include "elf/elf.h"
#include "elf/syms.h"
#include "elf/relocator.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

/** Default start address for loading code. */
#define DFLT_START_ADDR    0x1000

/** Max number of ELF sections to be read from ELF file. */
#define NUM_SECTIONS 4

/** Name of the sections to be read from ELF file. */
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"

/** Prefix for reloc section names. */
#define RELOC_PREFIX_STR ".rel"

/** 
 * @brief Check whether a string matches one in the ELF file symbol list. 
 * @param name Symbol to be searched for. 
 * @param symtab Symbol table. 
 * @return 1 in case there is a match, otherwise 0. 
 */
int is_in_symbols( char* name, stab symtab ) {
  unsigned int i;

  for ( i = 0; i < symtab.size; i++ ) {
    if ( 0 == strcmp( symtab.sym[ i ].name, name ) ) return 1;
  }
  return 0;
}

/*--------------------------------------------------------------------------  */
/**
 * @brief Find the number of segments to allocate (corresponding to section names). 
 * @param symtab Symbol table. 
 * @param section_names Names of sections to look for. 
 * @param nb_sections Number of sections to look for. 
 * @return Number of sections actually found. 
 */
unsigned int get_nsegments( stab symtab, char* section_names[], int nb_sections ) { 
  unsigned int n = 0;
  int i;
  
  for ( i = 0; i < nb_sections; i++ ) { 
    if ( is_in_symbols( section_names[ i ], symtab ) ) {
      n++;
    }
  }
  return n;
}

/**
 * @brief Extract given section from ELF file and load it to corresponding memory segment. 
 * @param fp Pointer to ELF file. 
 * @param memory Virtual memory structure. 
 * @param scn Name of the section to load. 
 * @param permissions Segment permissions (rights: Read, Write, eXecute). 
 * @param start_addr Virtual address the section should be loaded at. 
 *
 * @return  0 en cas de succes, une valeur non nulle sinon
 */
int elf_load_section_in_memory( FILE *fp, mem memory, char *scn, unsigned int permissions, unsigned long long start_addr ) { 
  byte   *ehdr    = __elf_get_ehdr( fp );
  byte   *content = NULL;
  uint    textsz  = 0;
  vsize   sz;
  vaddr   addr;
  int     scnidx; 
  
  byte *useless = elf_extract_section_header_table( ehdr, fp );
  free( useless );
  
  if ( NULL == ehdr ) {
    WARNING( "Can't read ELF file" );
    return 1;
  }
  
  if ( 1 == attach_scn_to_mem( memory, scn, SCN_ATTR( WIDTH_FROM_EHDR( ehdr ), permissions ) ) ) {
    WARNING( "Unable to create section %s.\n", scn );
    free( ehdr );
    return 1;
  }
  
  content = elf_extract_scn_by_name( ehdr, fp, scn, &textsz, NULL, &scnidx ); 
  if ( NULL == content ) {
    WARNING( "Corrupted ELF file.\n" );
    free( ehdr );
    return 1;
  }
  
  switch( WIDTH_FROM_EHDR(ehdr) ) {
  case 32 :
    sz._32   = textsz/*+8*/; /* +8: In case adding a final sys_exit is needed */
    addr._32 = start_addr; 
    break;
  case 64 :
    sz._64   = textsz/*+8*/; /* +8: In case adding a final sys_exit is needed */
    addr._64 = start_addr; 
    break;
  default :
    WARNING( "Wrong machine width.\n" );
    return 1;
  }

  if ( 1 == fill_mem_scn( memory, scn, sz, addr, content, scnidx ) ) {
    free( ehdr );
    free( content );
    WARNING( "Unable to fill in %s segment.\n", scn );
    return 1;
  }
  
  free( content );
  free( ehdr );
  
  return 0;
}

/**
 * @brief Relocate a given segment after segments have been loaded into virtual memory. 
 * @param fp Pointer to ELF file. 
 * @param seg Segment to be relocated. 
 * @param memory Virtual memory structure containing the segments. 
 * @param endianness ELF endianness.
 * @param symtab Symbol table. 
 * @todo Complete this function to actually perform relocation! 
 */
void reloc_segment( FILE* fp, segment seg, mem memory, unsigned int endianness, stab symtab ) {
  byte      *ehdr       = __elf_get_ehdr( fp );
  uint32_t   scnsz      = 0;
  Elf32_Rel *rel        = NULL;
  char      *reloc_name = calloc(1, strlen( seg.name ) + strlen( RELOC_PREFIX_STR ) + 1 ); 
  scntab     section_tab;
  
  /* Construct section name. */
  memcpy(reloc_name,RELOC_PREFIX_STR,strlen(RELOC_PREFIX_STR)+1);
  strcat(reloc_name,seg.name);
  
  /* Get reloc info and section table from ELF file. */
  rel = (Elf32_Rel *)elf_extract_scn_by_name( ehdr, fp, reloc_name, &scnsz, NULL, NULL );
  elf_load_scntab( fp, 32, &section_tab );
  
  
  /* TODO: Complete relocation code! */
  if ( rel != NULL && NULL != seg.content && seg.size._32 != 0 ) { 
    
    INFO( "-------------- Relocating %s (%lu symbols) -------------------\n", seg.name, scnsz / sizeof( *rel ) );
    
    /* TODO: Your relocation code goes here! */
    
    
  }
  
  del_scntab(section_tab);
  free( rel );
  free( reloc_name );
  free( ehdr );

  return;

  /* TODO: Remove these once you start using them! */
  UNUSED( memory );
  UNUSED( endianness );
  UNUSED( symtab.size );
  UNUSED( MIPS32_REL[ 0 ] );
}


/** 
 * @brief Print virtual memory segment bytes on standard output. 
 * @param seg Virtual memory segment to be printed. 
 */
void print_segment_raw_content( segment* seg ) { 
  unsigned int k;
  int          word = 0;
    
  if ( NULL != seg && seg->size._32 > 0 ) { 
    for( k = 0; k < seg->size._32; k += 4 ) { 
      if( k % 16 == 0 ) {
	printf( "\n  0x%08x ", k );
      }
      word = *((unsigned int *)(seg->content+k));
      FLIP_ENDIANNESS(word);
      printf( "%08x ", word ); 
    }
  }
}




/**
 * @brief The @c load command. Only requires slight analysis. 
 * @todo You must modify this function in order to actually load any ELF file!
 * @param interp The interpreter requesting the analysis. 
 * @return @ref CMD_RVAL_OK if loading of ELF file was successful, @ref CMD_RVAL_ERROR otherwise. 
 */
cmd_rval_t cmd_load( interpreter interp ) { 
  char        *elf_name = get_next_token( interp );
  char        *address  = NULL;
  addr         start;
  FILE        *elfp     = NULL;
  unsigned int nsegments; 
  unsigned int width; 
  unsigned int machine_type;
  endianness   endianness;
  char        *section_names[ NUM_SECTIONS ] = { TEXT_SECTION_STR, RODATA_SECTION_STR, DATA_SECTION_STR, BSS_SECTION_STR };
  unsigned int segment_permissions[ NUM_SECTIONS ] = { R_X, R__, RW_, RW_ };
  unsigned int next_segment_start; 
  unsigned int i = 0, j = 0;
  vsize stack_size;
  vaddr stack_start;

  
  /* First: parse command args. 
   * Syntax: load filename [address]
   */

  if ( NULL == elf_name ) {
    string err_loc;
    MAKE_ERR_LOC( err_loc, interp );
    ERROR( "Expected ELF file name as first arg of %s, got nothing (%s).\n",
	   CMD_NAME, err_loc ); 
    return CMD_RVAL_ERROR;    
  }
  
  if ( TOK_STRING != token_type( elf_name ) ) {
    string err_loc;
    MAKE_ERR_LOC( err_loc, interp );
    ERROR( "Expected ELF file name as first arg of %s, got '%s' (%s).\n",
	   CMD_NAME, elf_name, err_loc ); 
    return CMD_RVAL_ERROR;
  }

  address = get_next_token( interp );

  if ( NULL == address ) { 
    start._32 = DFLT_START_ADDR;
    INFO( "No loading address provided, defaulting to 0x%08x.\n", start._32 );
  }
  else {
    char *too_much_args = get_next_token( interp );
    if ( too_much_args ) {
      string err_loc;
      MAKE_ERR_LOC( err_loc, interp );
      ERROR( "Unexpected input for command '%s': '%s' (%s).\n",
	     CMD_NAME, too_much_args, err_loc );
      return CMD_RVAL_ERROR; 
    }
    
    if ( TOK_HEXA != token_type( address ) ) {
      string err_loc;
      MAKE_ERR_LOC( err_loc, interp );
      ERROR( "Expected hex address as second arg of %s, got '%s' (%s).\n",
	     CMD_NAME, address, err_loc ); 
      return CMD_RVAL_ERROR;
    }

    sscanf( address, "%x", &start._32 );

    /* Make sure we start on a 1K boundary. */
    start._32 = (start._32>>10)<<10;
    
    INFO( "Loading file starting from %s -> 0x%08x.\n", address, start._32 ); 
  }

  /* Second: Check ELF file. 
   */

  elfp = fopen( elf_name, "rb" );
  if ( NULL == elfp ) {
    string err_loc;
    MAKE_ERR_LOC( err_loc, interp );
    ERROR( "Unable to open '%s' for reading: %s (%s).\n",
	   elf_name, strerror( errno ), err_loc ); 
    return CMD_RVAL_ERROR;
  }

  if ( !assert_elf_file( elfp ) ) { 
    string err_loc;
    fclose( elfp );
    MAKE_ERR_LOC( err_loc, interp );
    ERROR( "File '%s' is not ELF-compliant (%s).\n",
	   elf_name, err_loc ); 
    return CMD_RVAL_ERROR;
  }
  
  /* Third: Load ELF file into interpreter. 
   */

  init_machine( &interp->mips ); 
  next_segment_start = start._32; 
    
  /* Get architecture info and symbols from ELF file. */
  elf_get_arch_info( elfp, &machine_type, &endianness, &width );
  elf_load_symtab( elfp, width, endianness, &interp->mips.symtab ); 
    
  nsegments = get_nsegments( interp->mips.symtab, section_names, NUM_SECTIONS ); 

  INFO( "Allocating %u segment%s from ELF file.\n", nsegments, nsegments > 1 ? "s" : "" ); 
  
  /* Allocate virtual memory */
  interp->mips.memory = init_mem( nsegments+1 );
  
  /* Do not forget to allocate sections. */
  j = 0;
  for ( i = 0; i < NUM_SECTIONS; i++ ) {
    if ( is_in_symbols( section_names[ i ], interp->mips.symtab ) ) { 
      elf_load_section_in_memory( elfp, interp->mips.memory, section_names[ i ], segment_permissions[ i ], next_segment_start ); 
      next_segment_start += ( (interp->mips.memory->seg[ j ].size._32+0x1000) >> 10 ) << 10; /* Round up to next 1K boundary */
      /* print_segment_raw_content( &interp->mips.memory->seg[ j ] ); */
      j++;
    }
  }
  
  for ( i = 0; i < nsegments; i++ ) {
    reloc_segment( elfp, interp->mips.memory->seg[ i ], interp->mips.memory, endianness, interp->mips.symtab ); 
  }
  
  /* TODO: Allocate .stack (and update number of segments!) */
  stack_size._32 = 0xfffff000-0xff7ff000;
  stack_size._64 = 0xfffff000-0xff7ff000;
  stack_start._32 = 0xff7ff000;
  stack_start._64 = 0xff7ff000;
  attach_scn_to_mem( interp->mips.memory, ".stack", SCN_ATTR(32 , RW_ ));
  fill_mem_scn( interp->mips.memory, ".stack", stack_size , stack_start, NULL, 5 );
  interp->mips.reg.registres_usuel[29] = 0xfffff000-4; //intialisation du pointeur sp sur l'adresse de début de la stack
  
  /*Place le registre PC à l'adresse du début de .text*/
  for(i=0;i<interp->mips.memory->nseg;i++){
    if(strcmp(interp->mips.memory->seg[i].name, ".text") == 0){
      interp->mips.reg.registres_usuel[34] = interp->mips.memory->seg[i].start._32;
    }
  }

  printf( "-------------- ELF file %s summary --------------\n", elf_name );
  print_mem( interp->mips.memory );
  stab32_print( interp->mips.symtab );
  
  

  /* Fourth: finish properly.
   */

  fclose( elfp ); 

  return CMD_RVAL_OK; 
}
