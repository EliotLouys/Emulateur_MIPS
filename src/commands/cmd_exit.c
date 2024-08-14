
/**
 * @file cmd_exit.c
 * @author Serge Le Thanh 
 * @date 16/09/2020
 * @brief Emulator command: exit.
 *
 * Emulator command: exit. 
 */

#include "commands.h"



cmd_rval_t cmd_exit( interpreter interp ) { 

char           *token = NULL;

token = get_next_token( interp );

if(token==0){
	return CMD_RVAL_EXIT; 
}
else {
	return CMD_RVAL_ERROR;
}

}


