
/**
 * @file section.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Nov  5 14:58:09 2013
 * @brief Section handling.
 *
 * Section handling.
 */

#include "notify.h"
#include "elf/section.h"


/**
 * @brief Creates a new table of sections and performs the necessary memory allocations. 
 * @param size The number of sections of this table. 
 * @return The new allocated table of sections. 
 */
scntab new_scntab( uint size ) {
    scntab scntab;

    scntab.size = size;

    scntab.scn  = malloc( scntab.size*sizeof( *(scntab.scn) ) );

    if ( NULL == scntab.scn ) {
        WARNING( "Unable to allocate section table.\n" );
    }

    return scntab;
}

/**
 * @brief Destroys a table of sections by making all the necessary free operations. 
 * @param scntab The table of sections to free. 
 */
void   del_scntab( scntab scntab ) {
    if ( NULL != scntab.scn ) {
        uint i;

        for ( i= 0; i< scntab.size; i++ ) {
            if ( NULL != scntab.scn[i].name ) {
                free( scntab.scn[i].name );
            }
        }

        free( scntab.scn );
    }

    return;
}


/**
* @brief Prints the content of a table of sections on the standard output. 
* @param scntab The table of sections to print. 
*/
void   print_scntab( scntab scntab ) {

    if ( NULL != scntab.scn ) {
        uint i;

        printf( "Listing %u sections:\n", scntab.size );

        for ( i= 0; i< scntab.size; i++ ) {
            printf( "Scn[%02u]: %s\n", i, scntab.scn[i].name );
        }
    }

    return;
}

