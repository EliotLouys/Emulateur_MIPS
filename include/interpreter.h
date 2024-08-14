
/**
 * @file interpreter.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Aug 19 00:04:41 2020
 * @brief Interpreter definitions.
 *
 * Interpreter definitions.
 */

#include <stdio.h>

#include "macros.h"
#include "machine.h"
#include "ctype.h"

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#ifdef __cplusplus
extern "C" {
#endif

  #define NB_FUNC 43

  /* Types */

  /**
   * @brief Interpreter modes.
   */
  typedef enum {
    INTERACTIVE, /**< Running in interactive mode (plain or debugging). */
    SCRIPT,       /**< Running in script mode. */
    DEBOG  /**Running in debog mode*/
  } interp_mode_t;

  /**
   * @brief Internal state of the interpreter. Feel free to add what you may need!
   */
  typedef struct {
    interp_mode_t   mode;                   /**< Current interpreter running mode. */
    FILE           *fp;                     /**< Current command input stream. */
    FILE           *save_fp;                /**< Used to switch input stream when debugging. */
    char           *script_file;            /**< Name of the script file being executed. */
    int             script_line;            /**< Current line of the script being executed. */
    char            input[ MAX_STR + 1 ];   /**< Current command being executed. */
    char           *from;                   /**< Internal use for @c strtok_r . */
    char            first_token;            /**< Internal use for @c strtok_r (whether we are scanning the first token or not). */
    machine         mips;                   /**< MIPS virtual machine. */
    union instruct_poly word_value;         /*stocke le mot d'instruction lu dans la mémoire*/
    int             dico;                   /*stocke la case du dictionnaire*/             
  } *interpreter;

  /**
   * @brief Token types
   * @todo You must complete this definition with your own additional types!
   */
  typedef enum {
    TOK_HEXA,            /**< Hexadecimal token. */
	TOK_OCTA,			/**<Octal token. */
	TOK_DEC,			/**<Decimal token. */
    TOK_STRING,           /**< String token (see @ref token_type). */
    TOK_REG   /**< Register token.*/
  } token_t;


  /* Prototypes */

  char         *get_next_token( interpreter interp );
  token_t       token_type( char *token );
  int is_hexa( char *token );
  int is_dec( char *token );
  int is_octal( char *token );

  interpreter   interp_new( void );
  void          interp_del( interpreter interp );
  void          real_addr_mem (interpreter interp, int *segm, int *indic, uint32_t virtual_addr, int *flag);
  void          swap (char *p1, char *p2);
  instruct_def  *lecture_dico();
  void          print_dico(instruct_def *inst);
  void          del_dico(instruct_def *inst);


#ifdef __cplusplus
}
#endif

#endif /* _INTERPRETER_H_ */
