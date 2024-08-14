
/**
 * @file cmd_bar.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Aug 18 23:59:35 2020
 * @brief Emulator command: bar.
 *
 * Emulator command: bar. 
 */

#include "commands.h"


/**
 * @brief A @c bar command you can use as a template to implement your own interpreter commands. 
 * In this toy example, the @c bar command expects one or more hex values. Its action is only
 * to print them on screen. 
 * @param interp The interpreter requesting the analysis. 
 * @return @ref CMD_RVAL_OK in case of success, @ref CMD_RVAL_ERROR otherwise. 
 */

cmd_rval_t cmd_bar( interpreter interp ) { 
  int             no_args = 1; 
  /* The 'bar' command expects hex integer values */
  unsigned int    hexval;
  char           *token = NULL;
  string          err_loc; /* To log error location. */

  /* Get first token (first parm of 'bar' command). */
  token = get_next_token( interp );
  
  /*
    The loop is executed while hex values can be read. 
    Notice the use of the 'no_arg' variable to detect the possible 
    lack of parameters. 
   */
  while ( NULL != token ) { 

    no_args = 0;
    
    switch ( token_type( token ) ) { 

    case TOK_HEXA:
      sscanf( token,"%x", &hexval ); /* We know we have a hex, so scan it! */
      fprintf( stdout, "Making something with value: 0x%x (dec: %u).\n", hexval, hexval ); 
      break;
      
    default :
      MAKE_ERR_LOC( err_loc, interp ); 
      ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	     token, CMD_NAME, err_loc ); 
      
      return CMD_RVAL_ERROR; 
    }
    
    token = get_next_token( interp );
  }
  
  if ( no_args ) {
    MAKE_ERR_LOC( err_loc, interp );
    ERROR( "No argument given to command '%s' (%s).\n", CMD_NAME, err_loc );
    return CMD_RVAL_ERROR;
  }
  
  return CMD_RVAL_OK; 
}


