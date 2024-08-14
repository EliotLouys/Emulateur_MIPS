/**
 * @file cmd_step.c
 * @author Serge Le Thanh
 * @date 30/10/2020
 * @brief Emulator command:step
 *
 * Emulator command: step
 */

#include "commands.h"
#include "bits.h"

cmd_rval_t cmd_step(interpreter interp, instruct_def *inst){
    char           *token = NULL;
    int 			flag1, segm1, indic1, b;
    uint32_t        PC_temp;
    string          err_loc; /* To log error location. */

    token = get_next_token( interp );

    if(interp->mips.memory == NULL){
	    MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Aucun fichier chargé %s\n", err_loc );
	    return CMD_RVAL_ERROR;
    }

    else if (token == 0){
        PC_temp = interp->mips.reg.registres_usuel[34]; //stockage de l'adresse du PC avant éxécution des instructions
        do{
            real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
            if(flag1!=0 ){
		        MAKE_ERR_LOC( err_loc, interp ); 
		        ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		        return CMD_RVAL_ERROR;
	        }   
            disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
            if(interp->dico == -1){
                MAKE_ERR_LOC( err_loc, interp ); 
		        ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		        return CMD_RVAL_ERROR;
            }
            b = execute_inst(interp, inst);//Exécution de l'instruction
            if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
                printf("Présence d'un breakpoint à l'adresse PC:0x%x\nRelancer le programme avec step ou step into\n", interp->mips.reg.registres_usuel[34]);
                return CMD_RVAL_OK;
            }
        }while(interp->mips.reg.registres_usuel[34] != (PC_temp + 4));
        return CMD_RVAL_OK;
    }

    else if(0 == strcmp(token, "into")){
        real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
        if(flag1!=0 ){
		    MAKE_ERR_LOC( err_loc, interp ); 
		    ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		    return CMD_RVAL_ERROR;
	    }
        disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
        if(interp->dico == -1){
                MAKE_ERR_LOC( err_loc, interp ); 
		        ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		        return CMD_RVAL_ERROR;
        }
        execute_inst(interp, inst);//Exécution de l'instruction
        return CMD_RVAL_OK;
    }

    else{
        MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Erreur d'argument: %s\n", err_loc );
	    return CMD_RVAL_ERROR;
    }

}