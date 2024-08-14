/**
 * @file emulmips.c
 * @author François Cayre, Nicolas Castagné, François Portet
 * @brief Starter code for the MIPS emulator.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* readline library, make sure it is installed on your system! */
#include <readline/readline.h>
#include <readline/history.h>

/* Useful constants and macros: DEBUG, WARNING, ERROR, UNUSED */
#include "macros.h"

/* Interpreter commands */
#include "commands.h"

/* Emulator MIPS32 instructions */
#include "insns.h"

/** We have one global variable: the verbose level (see @ref notify.h). */
int verbose_level = 0;


/*
 * Two important functions of the emulator:
 * execute_cmd : parse the input command and executes it by calling the associated function;
 * acquire_line: get one input line (hopefully, a valid interpreter command in the input
 * stream (script file of keyboard)
 */

/**
 * @brief Parse the current input string in search of a known command, which gets executed
 * in case it is found.
 * @todo Add other interpreter functions in this function!
 * @param interp The interpreter requesting the analysis.
 * @return @ref CMD_RVAL_OK if the command was successfully executed.
 * @return @ref CMD_RVAL_EXIT in the special case of the 'exit' command. Must lead to the
 * program termination.
 * @return @ref CMD_RVAL_UNKNOWN in case the command is not recognized as a valid one.
 * @return @ref CMD_RVAL_ERROR in case the command execution raised an error.
 */
cmd_rval_t execute_cmd( interpreter interp, instruct_def *inst ) {
  string  cmd_str; /* Temporary string for the test below */
  char   *token = NULL;
  string  err_loc;

  STRING_INIT( cmd_str );

  /* Macro DEBUG : uniquement si compil en mode 'debug' */
  if ( '\0' != interp->input[ 0 ] ) {
    DEBUG( "Input command string: '%s'.\n", interp->input );
  }

  /* Handling empty commands, comments, etc. */
  if ( strlen( interp->input ) == 0                || /* Is the input line non-empty?                 */
       sscanf( interp->input, "%s", cmd_str ) == 0 || /* Does it contain at least one 'word'?         */
       strlen( cmd_str ) == 0                      || /* Is this hypothetical first 'word' non-empty? */
       cmd_str[ 0 ] == '#' ) {                        /* Does it start with a #? (this is a comment!) */
    return CMD_RVAL_OK;
  }

  /* Get the command on the first call to get_next_token. */
  token = get_next_token( interp );

  /* Check for known commands. To be continuated! */
  if ( 0 == strcmp( token, "load" ) ) {
    return cmd_load( interp );
  }
  if ( 0 == strcmp( token, "bar" ) ) {
    return cmd_bar( interp );
  }
  if ( 0 == strcmp( token, "exit" ) ) {
	  return cmd_exit( interp );
  }
  if ( 0 == strcmp( token, "disp" ) ) {
	  return cmd_disp( interp );
  }
  if ( 0 == strcmp( token, "set" ) )  {
    return cmd_set( interp );
  }
  if ( 0 == strcmp( token, "debug" ) ) {
    return cmd_debug( interp );
  }
  if ( 0 == strcmp( token, "resume" ) ) {
    return cmd_resume( interp );
  }
  if ( 0 == strcmp( token, "assert" ) ) {
    return cmd_assert( interp );
  }
  if ( 0 == strcmp( token, "disasm" ) ) {
    return cmd_disasm( interp, inst);
  }
  if ( 0 == strcmp( token, "run" ) ) {
    return cmd_run( interp, inst);
  }
  if ( 0 == strcmp( token, "step" ) ) {
    return cmd_step( interp, inst);
  }
  if ( 0 == strcmp( token, "break" ) ) {
    return cmd_break( interp);
  }
  /* Alternatively, you may want to use the following macros: */
  /* CMD_CHECK_AND_EXEC( load ); */
  /* CMD_CHECK_AND_EXEC( bar ); */

  MAKE_ERR_LOC( err_loc, interp );
  ERROR( "Unknown command: '%s' (%s).\n", cmd_str, err_loc );
  return CMD_RVAL_UNKNOWN;
}

/**
 * @param in Input line (possibly badly written).
 * @param out Line in suitable form for further analysis.
 * @return nothing
 * @brief This function will prepare a line for further analysis.
 *
 * This function will prepare a line for further analysis and check for low-level syntax errors.
 * Colons, brackets and commas are surrounded with blanks; tabs are replaced with blanks.
 * Negative signs '-' get attached to the following token (e.g., "toto -   56" -> "toto -56").
 */
static void string_standardize( char *in, char *out ) { 

  unsigned int i, j;
  size_t       len = strlen( in );

  STRING_INIT( out ); 
  
  for ( j = 0, i = 0 ; i < len ; i++ ) { 
    
    /* Insert blanks around special characters */
    if ( ':' == in[ i ] || '+' == in[ i ] || '-' == in[ i ] ) { 
      out[ j++ ] = ' ';
      out[ j++ ] = in[ i ];
      out[ j++ ] = ' ';      
    }
    
    /* Remove blanks after negative sign */
    else if ( '-' == in[ i ] ) {
      out[ j++ ] = ' ';
      out[ j++ ] = in[ i ];
      while ( isblank( (int)in[ i+1 ] ) ) i++;
    }
    
    /* Insert one blank before comments */
    else if ( '#' == in[ i ] ) { 
      out[ j++ ] = ' ';
      out[ j++ ] = in[ i ];
    }
    /* translate tabs into white spaces*/
    else if ( isblank( (int)in[ i ] ) ) out[ j++ ]=' ';
    else out[ j++ ] = in[ i ];
  }
}


/**
 *
 * @brief Get the next line from the input stream fp.
 * If fp == stdin, use the readline lib to manage command history.
 *
 * @return 0 in case of success.
 * @return a non-zero integer in case no new line was successfully read.
 */
static int  acquire_line( interpreter interp ) {
  char *chunk = NULL;
  char   *ptr = NULL;

  STRING_INIT( interp->input );

  interp->first_token = 0;

  if ( SCRIPT == interp->mode ) { /* Read a new line from a script file */
    size_t chunk_len;

    chunk = calloc( MAX_STR+1, sizeof(*chunk) );

    interp->script_line++;

    if ( NULL == fgets( chunk, MAX_STR, interp->fp ) ) {
      /* Reached end of script, add an 'exit' command in case
	 none was given already. */
      strncpy( chunk, "exit", 5 );
    }

    chunk_len = strlen( chunk );

    /* If we have a Windows-style EOL, replace final '\r' with '\0' */
    if ( chunk_len > 1 && '\r' == chunk[ chunk_len - 2 ] ) {
      chunk[ chunk_len - 2 ] = '\0';
      chunk_len -= 2;
    }

    /* Remove final U*x '\n' */
    else if ( chunk_len > 0 && '\n' == chunk[ chunk_len - 1 ] ) {
      chunk[ chunk_len - 1 ] = '\0';
      chunk_len -= 1;
    }

    ptr = chunk;
  }
  else { /* INTERACTIVE interpreter mode, use readline lib */

    chunk = readline( PROMPT_STRING );

    if ( NULL == chunk ) { /* Ctrl-d means 'exit'! */
      fprintf( stdout, "^d\n" );
      chunk = strdup( "exit" );
    }
    else if ( 0 == strlen( chunk ) ) {
      /* Empty command... */
      free( chunk );
      return 0;
    }

    /* Skip leading blanks */
    ptr = chunk;
    while ( isblank( *ptr ) ) {
      ptr++;
    }

    /* Add command to history (call to readline lib) */
    if ( strlen( ptr ) ) {
      add_history( ptr );
    }
  }

  /* Standardize input string: put blanks where they are (most) needed */
  string_standardize ( ptr, interp->input );

  /* Our chunk somehow was dynamically allocated (readline(3) or calloc(3)), so free it in any case. */
  free( chunk );

  return 0;
}

/*************************************************
 * All things command line and interpreter core.
 *************************************************/

/**
 * @brief Print program usage.
 * @param command Name of the program executable.
 */
static void print_usage( char *command ) {
  char *ptr = command + strlen( command ) - 1;
  while ( ptr > command && '/' != ptr[ -1 ] ) ptr--;
  fprintf( stderr, "\nSynopsis \n\n" );
  fprintf( stderr, "  %s [script]* [-v[v*]] [-h|--help]\n\n", ptr );
  fprintf( stderr, "\nOptions \n\n" );
  fprintf( stderr, "  [-h|--help]\n" );
  fprintf( stderr, "\tIf -h or --help is present, print this help.\n\n" );
  fprintf( stderr, "  [script]\n" );
  fprintf( stderr, "\tIf no script file is given or legible, executes in interactive mode.\n\n" );
  fprintf( stderr, "  [-v[v*]]\n" );
  fprintf( stderr, "\tSet verbose level (1 = -v, 2 = -vv, 3 = -vvv). Default: 0, Max: 3.\n\n" );
  fprintf( stderr, "\nExamples \n\n" );
  fprintf( stderr, "  Launch in interactive mode: \n" );
  fprintf( stderr, "\t%s\n\n", ptr );
  fprintf( stderr, "  Launch in interactive mode with verbose level = 3: \n" );
  fprintf( stderr, "\t%s -vvv\n\n", ptr );
  fprintf( stderr, "  Execute script with verbose level = 1: \n" );
  fprintf( stderr, "\t%s script.emu -v\n\n", ptr );
}

/**
 * @brief Parse program arguments (help needed, verbose level, script file name).
 * @param interp Interpreter state.
 * @param argc Number of arguments.
 * @param argv Actual argument strings.
 */
static void parse_args( interpreter interp, int argc, char *argv[] ) {
  int        arg;
  int        script_count = 0;
  char      *script = NULL;

  /* By default: INTERACTIVE interpreter mode */
  interp->fp   = stdin;
  interp->mode = INTERACTIVE;

  /* First: Do we want some help? */
  for ( arg = 1 ; arg < argc ; arg++ ) {
    if ( 0 == strncmp( argv[ arg ], "-h", 2 ) || 0 == strncmp( argv[ arg ], "--help", 6 ) ) {
      print_usage( argv[ 0 ] );
      exit( EXIT_SUCCESS );
    }
  }

  /* Second: Find whether verbose mode is active */
  for ( arg = 1 ; arg < argc ; arg++ ) {
    if ( '-' == argv[ arg ][ 0 ] ) {
      char *opt = &argv[ arg ][ 1 ];

      while ( 'v' == *opt ) {
	verbose_level++;
	opt++;
      }
    }
  }

  if ( verbose_level > 3 ) verbose_level = 3;

  /* Self-test! */
  INFO( "Verbose level set to %d.\n", verbose_level );

  /* Third: Use first script that we can read */
  for ( arg = 1 ; arg < argc ; arg++ ) {
    if ( '-' != argv[ arg ][ 0 ] ) {
      FILE *fp = NULL;
      script_count++;
      script = argv[ arg ];
      fp     = fopen( script, "rt" );
      if ( NULL == fp ) {
	ERROR( "Unable to open '%s' for reading: %s.\n", script, strerror( errno ) );
	interp_del( interp );
	exit( EXIT_FAILURE );
      }
      else {
	interp->fp          = fp;
	interp->mode        = SCRIPT;
	interp->script_file = script;
	INFO( "Running script '%s'.\n", interp->script_file );
	break;
      }
    }
  }

  if ( stdin == interp->fp ) {
    fprintf( stdout, "\n   * Type Ctrl-d or exit to terminate the shell. Happy MIPS hacking!\n\n" );
  }

  return;
}


/**
 * @brief Entry point.
 * @param argc Number of command line arguments.
 * @param argv Actual command line arguments.
 * @return exit integer value (cast as a byte!)
 */
int main ( int argc, char *argv[] ) {

  interpreter interp = interp_new(); /* Allocate and init new interpreter state. */
  instruct_def *inst;

  inst = lecture_dico();//Read the disas instruction definition
  DEBUG("Fin de lecture du dico\n");
  DEBUG("Affichage du dico\n");
  print_dico(inst);//print the dico
  DEBUG("Fin d'affichage du dico\n");
  
  parse_args( interp, argc, argv );

  /* Loop while an input line can be read */
  while ( 0 == acquire_line( interp ) ) {

    /* Monitor command execution, check for errors. */
    switch( execute_cmd( interp, inst ) ) {

    case CMD_RVAL_OK:
      break;

    case CMD_RVAL_EXIT:
      /* Clean on exit */
      interp_del( interp );
      del_dico(inst);
      clear_history();
      exit( EXIT_SUCCESS );
      break;

    default:
      /* The command execution raised an error.
	 In SCRIPT mode, the emulator must be killed.
       */
      if ( SCRIPT == interp->mode ) {
	ERROR( "An error occurred. Aborting.\n");
	interp_del( interp );
  del_dico(inst);
	exit( EXIT_FAILURE );
      }
      break;
    }
  }

  if ( SCRIPT == interp->mode && feof( interp->fp ) ) {
    /* In SCRIPT mode, EOF cleanly terminates the emulator */
    INFO( "Finished executing %s.\n", interp->script_file );
    interp_del( interp );
    del_dico(inst);
    exit( EXIT_SUCCESS );
  }

  exit( EXIT_SUCCESS );
}
