/**
 * @file cmd_set.c
 * @author Eliot Louys
 * @date 16/09/2020
 * @brief Emulator command:resume
 *
 * Emulator command: resume
 */
 #include "commands.h"

 cmd_rval_t cmd_resume(interpreter interp)
 {
   string err_loc;
   
   if (interp->mode==INTERACTIVE)
   {
	 interp->fp = interp->save_fp;
     interp->mode=SCRIPT;
     return CMD_RVAL_OK;
   }
  else
  {
    MAKE_ERR_LOC(err_loc,interp);
    ERROR("Already in script mode");
    return CMD_RVAL_ERROR;
  }
 }
