
/**
 * @file mem.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Nov  1 01:37:19 2013
 * @brief Memory.
 *
 * Memory.
 */

#include <strings.h>
#include <string.h>

#include "mem.h"
#include "bits.h"
#include "notify.h"

/**
 * @brief Creates a virtual memory and performs the necessary memory allocations. 
 * @param nseg The number of segments that composes this virtual memory. 
 * @return The new allocated virtual memory or NULL in case of failure. 
 */
mem  init_mem( uint32_t nseg ) {

  mem vm = calloc( 1, sizeof( *vm ) );
  
  if ( NULL == vm ) {
    WARNING( "Unable to allocate host memory for vmem.\n" );
    return NULL;
  }
  else {
    uint i;
    
    vm->seg = calloc( nseg, sizeof( *(vm->seg) ) );
    if ( NULL == vm->seg ) {
      WARNING( "Unable to allocate host memory for vmem segment.\n" );
      free( vm );
      return NULL;
    }
    
    /* each segment is initialized to a null value
     * Note that though this is unnecessary since a calloc is used
     * this permits future evolution of the default initialization values
     */
    for ( i= 0; i< nseg; i++ ) {
      vm->seg[i].name      = NULL;
      vm->seg[i].content   = NULL;
      vm->seg[i].start._64 = 0x0;
      vm->seg[i].size._64  = 0x0;
      vm->seg[i].attr      = 0x0;
      vm->seg[i].scnidx    = -1; 
    }
    
    vm->nseg = nseg;
  }
  
  return vm;
}

/**
 * @brief Allocates an available segment in the virtual memory to the section named @c name.
 * @param vm The set of virtual memory segments. 
 * @param name The name of the segment (e.g., @c .text, @c .data, etc.)
 * @param attr An integer value corresponding to the access and execution permissions of the segment (@see mem.h). 
 * @return 0 in case of success, a non-null value otherwise. 
 */
int attach_scn_to_mem( mem vm, char *name, uint32_t attr ) {

  if ( NULL != vm ) {
    uint i;
    
    for ( i= 0; i< vm->nseg; i++ ) {
      if ( NULL == vm->seg[i].name ) {
	vm->seg[i].attr = attr;
	vm->seg[i].name = strdup( name );
	return 0;
      }
    }
    
    WARNING( "All vmem segments already allocated.\n" );
    return 1;
  }
  else {
    WARNING( "Virtual memory not initialized.\n" );
    return 1;
  }
}

/**
 * @brief Sets a segment with all the information about a section read from a ELF file. 
 * @param vm The virtual memory in which the segment to fill in can be found. 
 * @param name The name of the segment to look for (e.g., @c .text, @c .data, etc.)
 * @param sz The size of the section content (in bytes). 
 * @param start The virtual address in the virtual memory at which the beginning of the segment is located. 
 * @param content the real address at which the bytes of the section is located
 * @param scnidx Section number in ELF file. 
 * @return 0 in case of success, a non-null value otherwise. 
 */
int fill_mem_scn( mem vm, char *name, vsize sz, vaddr start, byte *content, int scnidx ) {

  if ( NULL != vm ) {
    uint i;
    
    for ( i= 0; i< vm->nseg; i++ ) {
      
      if ( 0 == strncasecmp( vm->seg[i].name, name, strlen(name) ) ) {

	vm->seg[i].scnidx          = scnidx;
	
	switch( SCN_WIDTH( vm->seg[i].attr ) ) {
	case 32 :
	  vm->seg[i].size._32      = sz._32;
	  vm->seg[i].start._32     = start._32;
	  vm->seg[i].content       = calloc( 1, sz._32 );
	  if ( NULL == vm->seg[i].content ) {
	    WARNING( "Unable to allocate host memory for vmem segment %s content.\n", name );
	    vm->seg[i].size._32  = 0;
	    vm->seg[i].start._32 = 0;
	    return 1;
	  }
	  if ( NULL != content ) {
	    memcpy( vm->seg[i].content, content, sz._32 );
	  }
	  return 0;
	  break;
	case 64 :
	  vm->seg[i].size._64      = sz._64;
	  vm->seg[i].start._64     = start._64;
	  vm->seg[i].content       = calloc( 1, sz._64 );
	  if ( NULL == vm->seg[i].content ) {
	    WARNING( "Unable to allocate host memory for vmem segment %s content.\n", name );
	    vm->seg[i].size._64  = 0;
	    vm->seg[i].start._64 = 0;
	    return 1;
	  }
	  if ( NULL != content ) {
	    memcpy( vm->seg[i].content, content, sz._64 );
	  }
	  return 0;
	  break;
	default :
	  WARNING( "Wrong width.\n" );
	  return 1;
	}
	return 0;
      }
    }
    
    WARNING( "Segment %s not attached yet.\n", name );
    return 1;
  }
  else {
    WARNING( "Virtual memory not initialized.\n" );
    return 1;
  }
}


/**
* @brief Prints the contents of a virtual memory. 
* @param vm Set of virtual memory segments. 
*/
void print_mem( mem vm ) {
  if ( NULL != vm ) {
    uint i;
    uint n = 0;
    
    for ( i= 0; i< vm->nseg; i++ ) {
      if ( UNDEF != SCN_RIGHTS( vm->seg[i].attr ) ) {
	n++;
      }
    }
    
    printf( "Virtual memory map (%u segments)\n", n );
    printf( "Segment \tRights\tVAddr\t\tSize (bytes)\tELF section\n" );
    
    for ( i= 0; i< vm->nseg; i++ ) {
      
      if ( UNDEF == SCN_RIGHTS( vm->seg[i].attr ) ) {
	continue;
      }
      
      printf( "%-8s\t", vm->seg[i].name );
      switch( SCN_RIGHTS( vm->seg[i].attr ) ) {
      case R__ :
	printf( "r--" );
	break;
      case RW_ :
	printf( "rw-" );
	break;
      case R_X :
	printf( "r-x" );
	break;
      default :
	printf( "???" );
      }
      printf( "\t" );
      switch( SCN_WIDTH( vm->seg[i].attr ) ) {
      case 32 :
	printf( "0x%08x", vm->seg[i].start._32 );
	break;
      case 64 :
	printf( "0x%016lx", (long unsigned int)vm->seg[i].start._64 );
	break;
      default :
	printf( "?????????" );
	break;
      }
      printf( "\t" );
      switch( SCN_WIDTH( vm->seg[i].attr ) ) {
      case 32 :
	printf( "%u", vm->seg[i].size._32 );
	break;
      case 64 :
	printf( "%lu", (long unsigned int)vm->seg[i].size._64 );
	break;
      default :
	printf( "?????????" );
	break;
      }
      
      if ( -1 != vm->seg[i].scnidx ) { 
	printf( "\t\t%d", vm->seg[i].scnidx );
      }
      
      printf( "\n" );
    }
  }
  
  return;
}


/**
 * @brief Destroys a virtual memory by making all the necessary free operations. 
 * @param vm Set of virtual memory segments. 
 */
void del_mem( mem vm ) {

  if ( NULL != vm ) {
    
    if ( NULL != vm->seg ) {
      uint i;
      
      for ( i= 0; i< vm->nseg; i++ ) {
	if ( NULL != vm->seg[i].content ) {
	  free( vm->seg[i].content );
	}
	if ( NULL != vm->seg[i].name ) {
	  free( vm->seg[i].name );
	}
      }
      
      free( vm->seg );
    }
    
    free( vm );
  }
  
  return;
}


