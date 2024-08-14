/**
 * @file cmd_set.c
 * @author Eliot Louys
 * @date 16/09/2020
 * @brief Emulator command: debug
 *
 * Emulator command: debug
 */
#include "commands.h"

cmd_rval_t cmd_debug(interpreter interp)
{
  string err_loc;
  if (interp->mode==SCRIPT){
	interp->save_fp = interp->fp;
	interp->fp = stdin;
    interp->mode=INTERACTIVE;
    return CMD_RVAL_OK;
  }

else
{
  MAKE_ERR_LOC(err_loc,interp);
  ERROR("Already in debug mode '%s' (%s).\n",CMD_NAME,err_loc);
  return CMD_RVAL_ERROR;
}
}
