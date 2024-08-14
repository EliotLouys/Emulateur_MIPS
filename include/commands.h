
/**
 * @file commands.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Aug 19 00:09:29 2020
 * @brief List of emulator commands.
 *
 * List of emulator commands.
 */

#include <interpreter.h>

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif

  /* Types. */


  /**
   * @brief Return value type for command evaluation functions. See @ref execute_cmd function.
   */
  typedef enum {
    CMD_RVAL_OK,       /**< What to return in case an interpreter command performed successfully. */
    CMD_RVAL_EXIT,     /**< Special value requesting termination of emulator execution. */
    CMD_RVAL_UNKNOWN,  /**< What to return in case an interpreter command produced an unknown result. */
    CMD_RVAL_ERROR     /**< What to return in case an interpreter command raised an error. */
  } cmd_rval_t;


  /* Prototypes. Do not forget to add prototypes of other commands here. */

  cmd_rval_t cmd_load( interpreter interp );
  cmd_rval_t cmd_bar( interpreter interp );
  cmd_rval_t cmd_exit( interpreter interp );
  cmd_rval_t cmd_disp(interpreter interp);
  cmd_rval_t cmd_set(interpreter interp);
  cmd_rval_t cmd_debug(interpreter interp);
  cmd_rval_t cmd_resume(interpreter interp);
  cmd_rval_t cmd_assert(interpreter interp);
  cmd_rval_t cmd_disasm(interpreter interp, instruct_def *inst);
  void disasm(interpreter interp, instruct_def *inst, int segm1, int indic1, int i);
  cmd_rval_t cmd_run(interpreter interp, instruct_def *inst);
  int execute_inst(interpreter interp, instruct_def *inst);
  cmd_rval_t cmd_step(interpreter interp, instruct_def *inst);
  cmd_rval_t cmd_break(interpreter interp);
  int is_break_point(interpreter interp, uint32_t addr);
  int del_break(interpreter interp, uint32_t addr);
  int del_all_break(interpreter interp);
  int add_break(interpreter interp, uint32_t addr);
  void print_break_list(interpreter interp);
  break_point list_del_first(break_point l);
  int list_find(break_point l, uint32_t addr);
  break_point list_remove_n(int n, break_point l);


#ifdef __cplusplus
}
#endif

#endif /* _COMMANDS_H_ */
