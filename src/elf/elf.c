
/**
 * @file elf.c
 * @author FranÃ§ois Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Nov  2 17:03:04 2013
 * @brief ELF format.
 *
 * ELF format.
 */

#include <strings.h>
#include <string.h>

#include "notify.h"
#include "bits.h"
#include "macros.h"
#include "elf/elf.h"
#include "elf/formats.h"

/**
 * @brief Swaps a relocation entry. 
 * @param rel The relocation entry to swap. 
 * @param width The size of the target processor word in bits. 
 * @param e The target endianness. 
 * @return The address of the swapped bytes (same as input). 
 */
byte *__Elf_Rel_flip_endianness( byte *rel, uint width, endianness e ) { 

  if ( e == get_host_endianness() ) return rel;
  
  if ( width == 32 ) {
    Elf32_Rel *e32 = (Elf32_Rel*)rel;
    
    FLIP_ENDIANNESS( e32->r_offset );
    FLIP_ENDIANNESS( e32->r_info );
  }
  
  if ( width == 64 ) {
    Elf64_Rel *e64 = (Elf64_Rel*)rel;
    
    FLIP_ENDIANNESS( e64->r_offset );
    FLIP_ENDIANNESS( e64->r_info );
  }
  
  return rel;
}

/**
 * @brief Swaps a symbol entry. 
 * @param sym The symbol entry to swap. 
 * @param width The size of the target processor word in bits. 
 * @param e The target endianness. 
 * @return The address of the swapped bytes (same as input). 
 */
static byte *__Elf_Sym_flip_endianness( byte * sym, uint width, endianness e ) {
  if ( e == get_host_endianness() ) return sym;
  
  if ( width == 32 ) {
    Elf32_Sym *e32 = (Elf32_Sym*)sym;
    
    FLIP_ENDIANNESS( e32->st_name );
    FLIP_ENDIANNESS( e32->st_value );
    FLIP_ENDIANNESS( e32->st_size );
    FLIP_ENDIANNESS( e32->st_shndx );
  }
  
  if ( width == 64 ) {
    Elf64_Sym *e64 = (Elf64_Sym*)sym;
    
    FLIP_ENDIANNESS( e64->st_name );
    FLIP_ENDIANNESS( e64->st_value );
    FLIP_ENDIANNESS( e64->st_size );
    FLIP_ENDIANNESS( e64->st_shndx );
  }
  
  return sym;
}


/**
 * @brief Swaps a section table entry. 
 * @param shdr The section table to swap. 
 * @param width The size of the target processor word in bits. 
 * @param e The target endianness. 
 * @return The address of the swapped section table (same as input). 
 */
static byte *__Elf_Shdr_flip_endianness( byte * shdr, uint width, endianness e ) {
  if ( e == get_host_endianness() ) return shdr;
  
  if ( width == 32 ) {
    Elf32_Shdr *e32 = (Elf32_Shdr*)shdr;
    
    FLIP_ENDIANNESS( e32->sh_name );
    FLIP_ENDIANNESS( e32->sh_type );
    FLIP_ENDIANNESS( e32->sh_flags );
    FLIP_ENDIANNESS( e32->sh_addr );
    FLIP_ENDIANNESS( e32->sh_offset );
    FLIP_ENDIANNESS( e32->sh_size );
    FLIP_ENDIANNESS( e32->sh_info );
    FLIP_ENDIANNESS( e32->sh_addralign );
    FLIP_ENDIANNESS( e32->sh_entsize );
  }
  
  if ( width == 64 ) {
    Elf64_Shdr *e64 = (Elf64_Shdr*)shdr;
    
    FLIP_ENDIANNESS( e64->sh_name );
    FLIP_ENDIANNESS( e64->sh_type );
    FLIP_ENDIANNESS( e64->sh_flags );
    FLIP_ENDIANNESS( e64->sh_addr );
    FLIP_ENDIANNESS( e64->sh_offset );
    FLIP_ENDIANNESS( e64->sh_size );
    FLIP_ENDIANNESS( e64->sh_info );
    FLIP_ENDIANNESS( e64->sh_addralign );
    FLIP_ENDIANNESS( e64->sh_entsize );
  }
  
  return shdr;
}

/**
 * @brief Swaps a ELF header. 
 * @param ehdr The header content to swap. 
 * @param width The size of the target processor word in bits. 
 * @param e The target endianness. 
 * @return The address of the swapped header content (same as input). 
 */
static byte *__Elf_Ehdr_flip_endianness( byte * ehdr, uint width, endianness e ) {

  if ( e == get_host_endianness() ) return ehdr;
  
  if ( width == 32 ) {
    Elf32_Ehdr *e32 = (Elf32_Ehdr*)ehdr;
    
    FLIP_ENDIANNESS( e32->e_type );
    FLIP_ENDIANNESS( e32->e_machine );
    FLIP_ENDIANNESS( e32->e_version );
    FLIP_ENDIANNESS( e32->e_entry );
    FLIP_ENDIANNESS( e32->e_phoff );
    FLIP_ENDIANNESS( e32->e_shoff );
    FLIP_ENDIANNESS( e32->e_flags );
    FLIP_ENDIANNESS( e32->e_ehsize );
    FLIP_ENDIANNESS( e32->e_phentsize );
    FLIP_ENDIANNESS( e32->e_phnum );
    FLIP_ENDIANNESS( e32->e_shentsize );
    FLIP_ENDIANNESS( e32->e_shnum );
    FLIP_ENDIANNESS( e32->e_shstrndx );
  }
  
  if ( width == 64 ) {
    Elf64_Ehdr *e64 = (Elf64_Ehdr*)ehdr;
    
    FLIP_ENDIANNESS( e64->e_type );
    FLIP_ENDIANNESS( e64->e_machine );
    FLIP_ENDIANNESS( e64->e_version );
    FLIP_ENDIANNESS( e64->e_entry );
    FLIP_ENDIANNESS( e64->e_phoff );
    FLIP_ENDIANNESS( e64->e_shoff );
    FLIP_ENDIANNESS( e64->e_flags );
    FLIP_ENDIANNESS( e64->e_ehsize );
    FLIP_ENDIANNESS( e64->e_phentsize );
    FLIP_ENDIANNESS( e64->e_phnum );
    FLIP_ENDIANNESS( e64->e_shentsize );
    FLIP_ENDIANNESS( e64->e_shnum );
    FLIP_ENDIANNESS( e64->e_shstrndx );
  }
  
  return ehdr;
}


/**
 * @brief Extracts the ELF header from ELF file. 
 * @param fp The pointer to the opened ELF file. 
 * @return The address of the raw content of the header in case of success, @c NULL otherwise. 
 */
byte *__elf_get_ehdr( FILE * fp ) {
  byte       *ehdr=NULL;
  byte        ident[ EI_NIDENT ];
  endianness  endian;
  
  if ( NULL == fp ) {
    WARNING( "No file to read from.\n" );
    return NULL;
  }
  
  rewind( fp );
  
  if ( fread( ident, EI_NIDENT, 1, fp ) < 1 ) {
    WARNING( "Corrupted ELF file.\n" );
    return NULL;
  }
  
  rewind( fp );
  endian = ident[EI_DATA]==ELFDATA2LSB?LSB:MSB;
  
  switch ( ident[ EI_CLASS ] ) {
  case ELFCLASS32 :
    ehdr = malloc( sizeof( Elf32_Ehdr ) );
    if ( fread( ehdr, sizeof( Elf32_Ehdr ), 1, fp ) < 1 ) {
      WARNING( "Corrupted ELF file.\n" );
      return NULL;
    }
    __Elf_Ehdr_flip_endianness( ehdr, 32, endian );
    return ehdr;
  case ELFCLASS64 :
    ehdr = malloc( sizeof( Elf64_Ehdr ) );
    if ( fread( ehdr, sizeof( Elf64_Ehdr ), 1, fp ) < 1 ) {
      WARNING( "Corrupted ELF file.\n" );
      return NULL;
    }
    __Elf_Ehdr_flip_endianness( ehdr, 64, endian );
    return ehdr;
  default :
    WARNING( "Invalid ELF class (must be 32 or 64).\n" );
    return NULL;
  }
  
  return ehdr;
}

/**
 * @brief Extracts section name table from ELF file. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param size OUTPUT The size in bytes of the extracted content. 
 * @return The address of the raw content of the section name table in case of success, @c NULL otherwise. 
 */
byte *elf_extract_section_names( FILE *fp, uint *size ) {
  byte *sstab  = NULL;
  byte *ehdr   = __elf_get_ehdr( fp );
  uint  endian = 0 ;
  uint  width  = 0 ;
  
  if ( NULL == ehdr ) {
    WARNING( "Can't read ELF file.\n" );
    return NULL;
  }
  
  endian = ENDIANNESS_FROM_EHDR( ehdr );
  width  = WIDTH_FROM_EHDR( ehdr );
  *size  = 1;
  
  if ( endian != ELFDATA2LSB && endian != ELFDATA2MSB ) {
    WARNING( "Wrong endianness.\n" );
    free( ehdr );
    return NULL;
  }
  
  if ( width != 32 && width != 64 ) {
    WARNING( "Wrong width.\n" );
    free( ehdr );
    return NULL;
  }
  
  if ( width == 32 ) {
    Elf32_Ehdr *e = (Elf32_Ehdr*)ehdr;
    
    if ( e->e_shstrndx == SHN_UNDEF ) {
      WARNING( "ELF file has no section names table.\n" );
      return NULL;
    }
    
    if ( 0 == e->e_shnum ) {
      WARNING( "File has no section header table.\n" );
      free( ehdr );
      return NULL;
    }
    
    fseek( fp, e->e_shstrndx*e->e_shentsize+e->e_shoff, SEEK_SET );
  }
  
  if ( width == 64 ) {
    Elf64_Ehdr *e = (Elf64_Ehdr*)ehdr;
    
    if ( e->e_shstrndx == SHN_UNDEF ) {
      WARNING( "ELF file has no section names table.\n" );
      free( ehdr );
      return NULL;
      
      
      if ( 0 == e->e_shnum ) {
	WARNING( "File has no section header table.\n" );
	free( ehdr );
	return NULL;
      }
      
      fseek( fp, e->e_shstrndx*e->e_shentsize+e->e_shoff, SEEK_SET );
    } /* CHACKE */
  }
  if ( width == 32 ) {
    Elf32_Shdr shdr;
    endianness e = endian==ELFDATA2LSB?LSB:MSB;
    
    if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
      WARNING( "Corrupted ELF file.\n" );
      free( ehdr );
      return NULL;
    }
    
    __Elf_Shdr_flip_endianness( (byte*)&shdr, 32, e );
    
    *size = shdr.sh_size;
    
    sstab = malloc( *size );
    
    if ( NULL == sstab ) {
      WARNING( "Can't allocate memory for section names table.\n" );
      free( ehdr );
      return NULL;
    }
    
    fseek( fp, shdr.sh_offset, SEEK_SET );
    
    if ( fread( sstab, *size, 1, fp ) < 1 ) {
      WARNING( "Corrupted ELF file.\n" );
      free( ehdr );
      return NULL;
    }
  }
  else {
    Elf64_Shdr shdr;
    endianness e = endian == ELFDATA2LSB?LSB:MSB;
    
    if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
      WARNING( "Corrupted ELF file.\n" );
      free( ehdr );
      return NULL;
    }
    
    __Elf_Shdr_flip_endianness( (byte*)&shdr, 64, e );
    
    *size = shdr.sh_size;
    
    sstab = malloc( *size );
    
    if ( NULL == sstab ) {
      WARNING( "Can't allocate memory for section names table.\n" );
      free( ehdr );
      return NULL;
    }
    
    fseek( fp, shdr.sh_offset, SEEK_SET );
    if ( fread( sstab, *size, 1, fp ) < 1 ) {
      WARNING( "Corrupted ELF file.\n" );
      free( sstab );
      free( ehdr );
      return NULL;
    }
  }
  free( ehdr );
  
  return sstab;
}


/**
 * @brief Extracts the content of a section using its section name. 
 * @param ehdr INPUT The ELF header data. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param name INPUT The name of the section to extract. 
 * @param secsz OUTPUT The size in bytes of the extracted content. 
 * @param file_offset OUTPUT The offset in bit from the beginning of the file where the raw content of the section is stored. 
 * @param scnidx OUTPUT Where to store the section index. 
 * @return The address of the allocated memory space in which the raw content of the section is stored in case of success, @c NULL otherwise. 
 */

byte *elf_extract_scn_by_name( byte *ehdr, FILE *fp, char *name, uint *secsz, uint *file_offset, int *scnidx ) { 
  uint        sstabsz    = 0;
  byte       *sstab      = elf_extract_section_names( fp, &sstabsz );
  uint        eendian    = ENDIANNESS_FROM_EHDR( ehdr );
  endianness  endian     = eendian==ELFDATA2LSB?LSB:MSB;
  uint        width      = WIDTH_FROM_EHDR( ehdr );
  byte       *section    = NULL;
  
  if ( NULL == sstab ) {
    WARNING( "Unable to read section names table.\n" );
    return NULL;
  }
  
  if ( width == 32 ) {
    Elf32_Ehdr *e      = (Elf32_Ehdr*)ehdr;
    Elf32_Shdr  shdr;
    uint        i;
    
    for ( i = 0; i < e->e_shnum; i++ ) { 
      fseek( fp, e->e_shoff+i*e->e_shentsize, SEEK_SET );
      
      if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
	WARNING( "Corrupted ELF file: i == %u %u %u.\n", i, e->e_shoff, e->e_shentsize );
	return NULL;
      }
      
      __Elf_Shdr_flip_endianness( (byte*)&shdr, 32, endian );
      if ( 0 == strncmp( (char*)&sstab[ shdr.sh_name ], name, strlen( name ) ) ) {
	break;
      }
    }
    
    if ( i == e->e_shnum ) {
      free( sstab );
      return NULL;
    }

    if ( scnidx ) { 
      *scnidx = i; 
    }
    
    section = calloc( shdr.sh_size, sizeof( *section ) ); 
    
    if ( NULL == section ) {
      WARNING( "Unable to allocate memory for section %s.\n", name );
      return NULL;
    }
    
    *secsz = shdr.sh_size;
    if ( file_offset )
      *file_offset = shdr.sh_offset;
    
    if ( SHT_NOBITS != shdr.sh_type ) {
      fseek( fp, shdr.sh_offset, SEEK_SET );
      if ( shdr.sh_size > 0 && fread( section, shdr.sh_size, 1, fp ) < 1 ) {
	WARNING( "Corrupted ELF file.\n" );
	return NULL;
      }
    }

    /* if ( strstr( name, "str" ) ) { */
    /*   int ii; */
    /*   DEBUG( "Section %s:\n", name ); */
    /*   for ( ii = 0 ; ii < shdr.sh_size ; ii++ ) { */
    /* 	if ( '\0' == section[ ii ] ) { */
    /* 	  fprintf( stderr, " - " ); */
    /* 	} */
    /* 	else { */
    /* 	  fprintf( stderr, "%c", section[ ii ] ); */
    /* 	} */
    /*   } */
    /*   fprintf( stderr, "\n" ); */
    /* } */

  }
  else {
    Elf64_Ehdr *e      = (Elf64_Ehdr*)ehdr;
    Elf64_Shdr  shdr;
    uint        i;
    
    for ( i= 0; i< e->e_shnum; i++ ) {
      fseek( fp, e->e_shoff+i*e->e_shentsize, SEEK_SET );
      
      if ( fread( &shdr, sizeof( shdr ), 1, fp ) < 1 ) {
	WARNING( "Corrupted ELF file.\n" );
	return NULL;
      }
      
      __Elf_Shdr_flip_endianness( (byte*)&shdr, 64, endian );
      
      if ( !strncasecmp( (char*)&sstab[shdr.sh_name], name, strlen(name) ) ) {
	break;
      }
    }
    
    if ( i == e->e_shnum ) {
      WARNING( "No section by that name.\n" );
      free( sstab );
      return NULL;
    }
    
    section = malloc( shdr.sh_size );
    
    if ( NULL == section ) {
      WARNING( "Unable to allocate memory for section %s.\n", name );
      return NULL;
    }
    
    *secsz = shdr.sh_size;
    if ( file_offset )
      *file_offset = shdr.sh_offset;
    
    if ( SHT_NOBITS != shdr.sh_type ) {
      fseek( fp, shdr.sh_offset, SEEK_SET );
      if ( fread( section, shdr.sh_size, 1, fp ) < 1 ) {
	WARNING( "Corrupted ELF file.\n" );
	return NULL;
      }
    }
  }
  
  free( sstab );

  return section;
}


/**
 * @brief Extracts the content of the table of strings. 
 * @param ehdr INPUT The ELF header data. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param name INPUT The name of the section to extract. 
 * @param sz OUTPUT The size in bytes of the extracted content. 
 * @return The address of the allocated memory space in which the raw content of the section is stored in case of success, @c NULL otherwise. 
 */
byte *elf_extract_string_table( byte *ehdr, FILE *fp, char *name, uint *sz ) {
  return elf_extract_scn_by_name( ehdr, fp, name, sz, NULL, NULL );
}

/**
 * @brief Prints the string table on the standard output. 
 * @param stab The raw string table content. 
 * @param sz The size in bytes of the string table. 
 */
void  print_string_table( byte *stab, unsigned int sz ) {
  unsigned int i = 1;
  
  while ( i< sz ) {
    printf( "String[%03u]: %s\n", i, (char*)&stab[i] );
    i += strlen( (char*)&stab[i] ) + 1;
  }
  
  return;
}


/**
 * @brief Extracts the section header table. 
 * @param ehdr The ELF header data. 
 * @param fp The pointer to the opened ELF file. 
 * @return The address at which the raw data is stored in case of success, @c NULL otherwise. 
 */
byte *elf_extract_section_header_table( byte * ehdr, FILE * fp ) {
  uint        sz      = 0;
  uint        width   = WIDTH_FROM_EHDR( ehdr );
  uint        eendian = ENDIANNESS_FROM_EHDR( ehdr );
  
  if ( 32 == width ) {
    Elf32_Ehdr *e     = (Elf32_Ehdr*)ehdr;
    Elf32_Shdr *s     = NULL;
    Elf32_Shdr *shtab = NULL;
    uint i;
    
    if ( 0 == e->e_shnum  ) {
      WARNING( "No section header table to extract.\n" );
      return NULL;
    }
    
    sz = e->e_shnum*sizeof(*s);
    
    shtab = malloc( sz );
    
    
    fseek( fp, e->e_shoff, SEEK_SET );
    if ( fread( shtab, e->e_shnum, sizeof(*s), fp ) < e->e_shnum ) {
      WARNING( "Could not read all shtab entries.\n" );
    }
    
    for ( i= 0; i< e->e_shnum; i++ ) {
      __Elf_Shdr_flip_endianness( (byte*)&shtab[i], 32, eendian );
    }
    
    return (byte*)shtab;
  }
  else {
    Elf32_Shdr *shtab = NULL;
    Elf64_Ehdr *e = (Elf64_Ehdr*)ehdr;
    
    if ( 0 < e->e_shnum  ) {
      WARNING( "No section header table to extract.\n" );
      return NULL;
    }
    
    sz = e->e_shnum*e->e_shentsize;
    
    shtab = malloc( sz );
    
    
    fseek( fp, e->e_shoff, SEEK_SET );
    if ( fread( shtab, e->e_shnum, e->e_shentsize, fp ) < e->e_shnum ) {
      WARNING( "Could not readd all shtab entries.\n" );
    }
    
    return (byte*)shtab;
  }
}


/**
 * @brief Extracts the symbol table. 
 * @param ehdr INPUT The ELF header data. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param name INPUT The name of the section. 
 * @param nsymbol OUTPUT The number of symbols in the table. 
 * @return The address at which the raw symbol table is stored in case of success, @c NULL otherwise. 
 */
byte *elf_extract_symbol_table( byte *ehdr, FILE *fp, char *name, unsigned int *nsymbol ) {
  uint        sz         = 0;
  byte       *symtab     = elf_extract_scn_by_name( ehdr, fp, name, &sz, NULL, NULL );
  uint        width      = WIDTH_FROM_EHDR( ehdr );
  
  if ( NULL == symtab ) {
    WARNING( "No symbol table found.\n" );
    return NULL;
  }
  
  if ( width == 32 ) {
    *nsymbol = sz/sizeof(Elf32_Sym);
    return symtab;
  }
  
  if ( width == 64 ) {
    *nsymbol = sz/sizeof(Elf64_Sym);
    return symtab;
  }
  
  return symtab;
}


/**
 * @brief Checks whether a given file is an ELF file. 
 * @param fp The pointer to the opened file. 
 * @return 0 if not an ELF file, a non-null value otherwise. 
 */
int assert_elf_file( FILE *fp ) {

  if ( NULL != fp ) {
    Elf32_Ehdr ehdr;
    
    rewind( fp );
    
    if ( fread( &ehdr, sizeof(ehdr), 1, fp ) < 1 ) {
      WARNING( "Can't read ELF file header.\n" );
      return 0;
    }
    
    return ( ehdr.e_ident[EI_MAG0] == 0x7f &&
	     ehdr.e_ident[EI_MAG1] == 'E'  &&
	     ehdr.e_ident[EI_MAG2] == 'L'  &&
	     ehdr.e_ident[EI_MAG3] == 'F'     );
  }
  
  return 0;
}

/**
 * @brief Gives the details of the architecture targeted by a given ELF file. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param machine OUTPUT The family of processors this code is intended to (@see formats.h). 
 * @param endianness OUTPUT The order with which bytes or a word are written/read in memory (see @ref types.h). 
 * @param width OUTPUT The size of the target processor word in bits. 
 * @return 0 in case of success, a non-null value otherwise. 
 */
int elf_get_arch_info( FILE *fp, uint *machine, endianness *endianness, uint *width ) {

  if ( NULL != fp ) {
    Elf32_Ehdr ehdr;
    
    rewind( fp );
    
    if ( fread( &ehdr, sizeof(ehdr), 1, fp ) < 1 ) {
      WARNING( "Can't read ELF file header.\n" );
      return 1;
    }
    
    switch( ehdr.e_ident[EI_DATA] ) {
    case ELFDATA2LSB :
      *endianness = LSB;
      break;
    case ELFDATA2MSB :
      *endianness = MSB;
      break;
    default :
      *endianness = UNDEF;
      break;
    }
    
    switch( ehdr.e_ident[EI_CLASS] ) {
    case ELFCLASS32 :
      *width = 32;
      break;
    case ELFCLASS64 :
      *width = 64;
      break;
    default :
      *width = 0;
      break;
    }

    __Elf_Ehdr_flip_endianness( (byte*)&ehdr, *width, *endianness );
    
    switch( ehdr.e_machine ) {
    case EM_NONE :
    case EM_M32 :
      *machine = MACH_T_UNDEF;
      break;
    case EM_SPARC :
      *machine = MACH_T_SPARC;
      break;
    case EM_386 :
      *machine = MACH_T_I386;
      break;
    case EM_68K :
      *machine = MACH_T_M68K;
      break;
    case EM_88K :
    case EM_860 :
      *machine = MACH_T_UNDEF;
      break;
    case EM_MIPS :
      *machine = MACH_T_MIPS;
      break;
    default :
      *machine = MACH_T_UNDEF;
      break;
    }
    
    return 0;
  }
  
  return 1;
}

/**
 * @brief Extracts the section table. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param width The size of the target processor word in bits. 
 * @param scntab OUTPUT The memory space in with the section table must be stored. 
 * @return 0 in case of success, a non-null value otherwise. 
 */
int elf_load_scntab( FILE *fp, unsigned int width, scntab *scntab ) {

  if ( 32 == width ) {
    Elf32_Ehdr *ehdr     = (Elf32_Ehdr*)__elf_get_ehdr( fp );
    uint        nscns    = NULL!=ehdr?ehdr->e_shnum:0;
    Elf32_Shdr *elf_shdr = (Elf32_Shdr*)elf_extract_section_header_table( (byte*)ehdr, fp );
    
    if ( NULL == elf_shdr ) {
      WARNING( "Unable to extract section header table.\n" );
      free( ehdr );
      return 1;
    }
    else {
      uint  i;
      uint  sz;
      char *scnname = (char*)elf_extract_scn_by_name( (byte*)ehdr, fp, ".shstrtab", &sz, NULL, NULL );
      
      if ( NULL == scnname ) {
	WARNING( "Unable to extract section name table.\n" );
	free( ehdr );
	free( elf_shdr );
	return 1;
      }
      
      *scntab = new_scntab( nscns );
      
      for ( i = 0; i < nscns; i++ ) {
	scntab->scn[ i ].name = strdup( &scnname[ elf_shdr[ i ].sh_name ] );
	scntab->scn[ i ].addr._32 = elf_shdr[ i ].sh_addr;
      }
      
      free( ehdr );
      free( elf_shdr );
      free( scnname );
      
      /*
	print_scntab(scntab );
      */
      return 0;
    } /* has shdr */
  } /* 32 == width */
  
  return 0;
}



/**
 * @brief Load the symbol table into a symbol table structure. 
 * @param fp INPUT The pointer to the opened ELF file. 
 * @param width INPUT The size of the target processor word in bits. 
 * @param endian INPUT The byte order of the targeted architecture of the ELF file. 
 * @param symtab OUTPUT The memory space in with the data must be stored. 
 * @return 0 in case of success, a non-null value otherwise. 
 */
int elf_load_symtab( FILE *fp,  unsigned int width, unsigned int endian,  stab *symtab ) {

  /* remove all previous allocation */
  if ( NULL == symtab ) { 
    ERROR( "Cannot load symbols in an unallocated memory space.\n" );
    return -1; 
  }
  
  del_stab( *symtab );
  
  
  if ( 32 == width ) {
    uint        nsyms   = 0;
    Elf32_Ehdr *ehdr    = (Elf32_Ehdr*)__elf_get_ehdr( fp );
    Elf32_Sym  *elf_sym = (Elf32_Sym*)elf_extract_symbol_table( (byte*)ehdr, fp, ".symtab", &nsyms );
    
    if ( 0 == nsyms ) {
      *symtab = new_stab( 0 );
      free( ehdr );
      free( elf_sym );
      return 0;
    }
    else {
      uint   i;
      uint   namesz  = 0;
      uint   sz      = 0;
      char  *snames  = (char*)elf_extract_scn_by_name( (byte*)ehdr, fp, ".strtab", &namesz, NULL, NULL );
      char  *scnname = (char*)elf_extract_scn_by_name( (byte*)ehdr, fp, ".shstrtab", &sz, NULL, NULL );
      Elf32_Shdr *scntab = (Elf32_Shdr*)elf_extract_section_header_table( (byte*)ehdr, fp );
      
      if ( NULL == scnname ) {
	WARNING( "Unable to extract section name table.\n" );
	free( ehdr );
	free( elf_sym );
	free( scntab );
	return 1;
      }
      
      if ( NULL == snames ) {
	WARNING( "Unable to read ELF symbol names.\n" );
	free( scnname );
	free( ehdr );
	free( elf_sym );
	free( scntab );
	return 1;
      }
      
      *symtab = new_stab( nsyms );
      
      for ( i = 0; i < nsyms; i++ ) { 
	sym_type t = STT_NOTYPE;
	uint     shndx;
		
	__Elf_Sym_flip_endianness( (byte*)&elf_sym[i], 32, endian );
	
	shndx = elf_sym[i].st_shndx;
	
	switch( ELF32_ST_TYPE( elf_sym[i].st_info ) ) {
	case STT_NOTYPE :
	  t = notype;
	  break;
	case STT_OBJECT :
	  t = object;
	  break;
	case STT_FUNC :
	  t = function;
	  break;
	case STT_SECTION :
	  t = section;
	  break;
	case STT_FILE :
	  t = file;
	  break;
	}
	
	
	/* printf( "ST_NAME[%02u] info %d: %u : %s \n", i,t, elf_sym[i].st_name, &snames[elf_sym[i].st_name] ); */
		
	if ( section == t ) {
	  symtab->sym[ i ] = new_sym32( &scnname[ scntab[ shndx ].sh_name ], elf_sym[ i ].st_value, elf_sym[ i ].st_size, t, shndx );
	}
	else {
	  symtab->sym[ i ] = new_sym32( &snames[ elf_sym[ i ].st_name ], elf_sym[ i ].st_value, elf_sym[ i ].st_size, t, shndx );
	  
	}
      }
      
      free( snames );
      free( scnname );
      free( scntab );
    } /* non-empty symtab */
    free( ehdr );
    free( elf_sym );
    
  } /* 32 == width */
  
  return 0;

  UNUSED( MIPS32_REL[ 0 ] ); /* Silence warning. */
}
