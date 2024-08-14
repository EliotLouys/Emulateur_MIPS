
/**
 * @file macros.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Aug 18 17:55:21 2020
 * @brief Convenience macros.
 *
 * Convenience macros.
 */

#include <string.h>
#include <stdio.h>

#include "types.h"

#ifndef _MACROS_H_
#define _MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief Init a string containing @ref MAX_STR usable chars.
   * The string must be declared (static string) or dynamically allocated
   * beforehand. @see string . 
   * @param str String to be initialized. 
   */  
#define STRING_INIT( str ) memset( (str), '\0', MAX_STR + 1 )

  /**
   * @brief Depending on the interpreter mode, print relevant error 
   * location information in the first argument string. In @ref SCRIPT 
   * mode, print the script file name and the line number at which the 
   * error occurred. Otherwise, print @c from @c keyboard. Synopsis: use 
   * before calling the @ref ERROR macro. 
   * @param str String where to write the location information.
   * @param interp Interpreter that encountered an error. 
   */
#define MAKE_ERR_LOC( str, interp ) do {				\
  STRING_INIT( str );							\
  if ( SCRIPT == interp->mode ) {					\
    snprintf( str, MAX_STR, "at %s:%d",					\
	      interp->script_file, interp->script_line );		\
  }									\
  else {								\
    snprintf( str, MAX_STR, "from keyboard" );				\
  }									\
  } while ( 0 )

  /**
   * @brief Expands to interpreter function name from C function name. 
   * If the C function is called @c cmd_foo, returns @c foo (skip the 
   * 4 chars @c cmd_ ). 
   */
#define CMD_NAME __FUNCTION__ + 4

  /**
   * @brief For use in @ref execute_cmd function only! Wraps test-and-execute 
   * logic for easier reading. 
   * @param name Name of the command to be added into the interpreter logic. 
   */
#define CMD_CHECK_AND_EXEC( name )		\
  if ( 0 == strcmp( token, #name ) ) {		\
    return cmd_##name( interp );		\
  }

  /**
   * @brief Silence compiler warning when some function parameter
   * (for example) will obviously not be used. Best to put after 
   * @c return so that no code is generated. 
   * @param thing The unused thing (numeral or pointer). 
   */
#define UNUSED( thing ) if ( thing ) {		\
  }


#ifdef __cplusplus
}
#endif

#endif /* _MACROS_H_ */
