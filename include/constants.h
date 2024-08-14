
/**
 * @file constants.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Aug 18 21:31:35 2020
 * @brief Constants.
 *
 * Constants.
 */

#include "notify.h"

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#ifdef __cplusplus
extern "C" {
#endif


  /** Prompt string for the interactive mode. */
#define PROMPT_STRING "\x1B[1;30m"">>""\x1B[0m"" "

  /** 
   * Maximum number of useful chars for static strings. 
   * Declare with: @code char mystr[ MAX_STR + 1 ]; @endcode or with:
   * @code string mystr; @endcode (see @ref string ).
   * Init with: @code memset( mystr, '\0', MAX_STR + 1 ); @endcode or 
   * with: @code STRING_INIT( mystr ); @endcode.
   */
#define MAX_STR 1023
  
#ifdef __cplusplus
}
#endif

#endif /* _CONSTANTS_H_ */
