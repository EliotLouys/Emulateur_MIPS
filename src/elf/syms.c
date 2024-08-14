
/**
 * @file syms.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 13:45:50 2013
 * @brief Binary symbols.
 *
 * Binary symbols.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "notify.h"
#include "elf/syms.h"


/**
 * @brief Creates a new symbol for a 32 bits target and returns it. 
 * @param name A string containing the name of the symbol. 
 * @param addr The virtual address (32 bits) in the virtual memory this symbol refers to. 
 * @param size The size of this symbol. 
 * @param type The kind of object this symbol refers to @see syms.h. 
 * @param scnidx The index in the ELF file to which this symbol belongs. 
 * @return A @ref sym structure initialised with these arguments. 
 */
sym  new_sym32( char *name, vaddr32 addr, uint size, sym_type type, uint scnidx ) {
  sym s;

  s.name        = strdup( name );
  s.addr._32    = addr;
  s.size        = size;
  s.type        = type;
  s.scnidx      = scnidx;
  s.scnidx_link = scnidx;
  
  return s;
}


/**
 * @brief Creates a new symbol for a 64 bits target and returns it. 
 * @param name A string containing the name of the symbol. 
 * @param addr The virtual address (64 bits) in the virtual memory this symbol refers to. 
 * @param size The size of this symbol. 
 * @param type The kind of object this symbol refers to @see syms.h. 
 * @param scnidx The index in the ELF file to which this symbol belongs. 
 * @return A @ref sym structure initialised with these arguments. 
 */
sym  new_sym64( char *name, vaddr64 addr, uint size, sym_type type, uint scnidx ) {
  sym s;
  
  s.name        = strdup( name );
  s.addr._64    = addr;
  s.size        = size;
  s.type        = type;
  s.scnidx      = scnidx;
  s.scnidx_link = scnidx;

  return s;
}

/**
* @brief Prints the contents of a 32-bit symbol on the standard output. 
* @param s A 32-bit symbol. 
*/
void sym32_print( sym s ) {

  printf( "Symbol %16s (%-4u bytes @ 0x%08x): ", s.name, s.size, s.addr._32 );
  
  switch( s.type ) {
  case notype :
    printf( "no type" );
    break;
  case object :
    printf( "object" );
    break;
  case function :
    printf( "function" );
    break;
  case section :
    printf( "section" );
    break;
  case file :
    printf( "file" );
    break;
  case linked :
    printf( "linked" );
    break;
  default :
    printf( "unknown type" );
    break;
  }
  
  if ( section != s.type && s.scnidx ) { 
    printf( ", relative to ELF section %u", s.scnidx );
  }
  
  printf( "\n" );
  
  return;
}

/**
 * @brief Prints the contents of a 64-bit symbol on the standard output. 
 * @param s A 64-bit symbol. 
 */
void sym64_print( sym s ) {

  printf( "Symbol %18s (%-4u bytes @ 0x%16lx): ", s.name, s.size, (long unsigned int)s.addr._64 );
  
  switch( s.type ) {
  case notype :
    printf( "no type" );
    break;
  case object :
    printf( "object" );
    break;
  case function :
    printf( "function" );
    break;
  case section :
    printf( "section" );
    break;
  case file :
    printf( "file" );
    break;
  case linked :
    printf( "linked" );
    break;
  default :
    printf( "unknown type" );
    break;
  }
  
  if ( section != s.type && s.scnidx ) { 
    printf( ", relative to ELF section %u", s.scnidx );
  }
  
  printf( "\n" );
  
  return;
}


/**
 * @brief Creates a new table of symbols and performs the necessary memory allocations. 
 * @param size The number of symbols of this table. 
 * @return The new allocated table of symbols. 
 */
stab new_stab( uint size ) {
  stab st;
  
  st.size = size;
  st.sym  = calloc( 1, st.size*sizeof( *(st.sym) ) );
  
  if ( NULL == st.sym ) {
    WARNING( "Unable to allocate symbol table.\n" );
    return st;
  }
  
  return st;
}


/**
 * @brief Find the first occurrence of a symbol corresponding to the input string in the symbol table. 
 * @param name The name of the symbole to find. 
 * @param s The symbol table to be searched. 
 * @return -1 if the symbol is not found, the index of the symbol in the table otherwise. 
 */
int get_sym_by_name(char* name, stab s ) {
  int i,index =-1;
  
  if ( NULL != s.sym ) {
    for ( i = 1; i < (int)s.size; i++ ) {
      if ( s.sym[ i ].name && 0 == strcmp( name, s.sym[ i ].name ) ) return i;
    }
    
  }
  
  return index;
}



/**
* @brief Prints the content of a table of 32-bit symbols on the standard output. 
* @param s The table of 32-bit symbols to be printed. 
*/
void stab32_print( stab s ) {

    if ( NULL == s.sym ) {
        return;
    }
    else {
        uint i;

        printf( "Symbol table has %u entries:\n", s.size-1 );
        for ( i = 1; i < s.size; i++ ) {
            sym32_print( s.sym[ i ] );
        }	
        return;
    }
}

/**
* @brief Prints the content of a table of 64-bit symbols on the standard output. 
* @param s The table of 64-bit symbols to be printed. 
*/
void stab64_print( stab s ) {

  if ( NULL == s.sym ) {
    return;
  }
  else {
    uint i;
    
    printf( "Symbol table has %u entries:\n", s.size-1 );
    for ( i = 1; i < s.size; i++ ) { 
      sym64_print( s.sym[ i ] );
    }
    return;
  }
}


/**
 * @brief Destroys a table of symbols by making all the necessary free operations. 
 * @param s The table of symbols to free. 
 */
void del_stab( stab s ) {

  if ( NULL != s.sym ) {
    uint i;
    
    for ( i = 0; i < s.size; i++ ) {
      
      if ( NULL != s.sym[ i ].name ) {
	free( s.sym[ i ].name );
      }
    }
    free( s.sym );
  }
  return;
}
