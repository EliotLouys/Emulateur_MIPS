/**
 * @file cmd_run.c
 * @author Serge Le Thanh
 * @date 29/10/2020
 * @brief Emulator command:run
 *
 * Emulator command: run
 */

#include "commands.h"
#include "ins_ass.h"
#include "bits.h"

cmd_rval_t cmd_run(interpreter interp, instruct_def *inst){
    char           *token = NULL;
    char 		   *endptr=NULL;
    uint32_t 		addr1;
    int 			flag1, segm1, indic1, res_exe;
    string          err_loc; /* To log error location. */

    token = get_next_token( interp );

    if(interp->mips.memory == NULL){
	    MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Aucun fichier chargé %s\n", err_loc );
	    return CMD_RVAL_ERROR;
    }

    else if (token == 0){
        interp->mips.reg.registres_usuel[34] = interp->mips.reg.registres_usuel[34]; //Execution du programme a partir de l'adresse actuelle du PC
    }

    else if(token_type(token)==TOK_HEXA){
        addr1 = (uint32_t) strtol(token, &endptr, 16);
        real_addr_mem (interp, &segm1, &indic1, addr1, &flag1);
        if(flag1!=0 ){
		    MAKE_ERR_LOC( err_loc, interp ); 
		    ERROR( "L'adresse chargée n'existe pas ou n'est pas allouée\n");
		    return CMD_RVAL_ERROR;
	    }
        else if(0 != strcmp(interp->mips.memory->seg[segm1].name, ".text")){
            MAKE_ERR_LOC( err_loc, interp ); 
		    ERROR( "L'adresse chargée dans le PC n'est pas dans le segment .text\n");
		    return CMD_RVAL_ERROR;
        }
        interp->mips.reg.registres_usuel[34] = addr1; //Chargement dans le PC de l'adresse d'éxécution
    }
    else{
        MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Erreur d'argument:%s\n", err_loc );
	    return CMD_RVAL_ERROR;
    }

    do{//Exécution du programme jusqu'à la fin des instructions ou d'un breakpoint
        real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
        uint32_t prev_addr = interp->mips.reg.registres_usuel[34];
        if(flag1!=0 ){
		    MAKE_ERR_LOC( err_loc, interp ); 
		    ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		    return CMD_RVAL_ERROR;
	    }
        if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
            printf("Présence d'un breakpoint à l'adresse PC:0x%x\nRelancer le programme avec step ou step into\n", interp->mips.reg.registres_usuel[34]);
            return CMD_RVAL_OK;
        }
        else{
            disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
            if(interp->dico == -1){
                MAKE_ERR_LOC( err_loc, interp ); 
		        ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		        return CMD_RVAL_ERROR;
            }
            res_exe = execute_inst(interp, inst);//Exécution de l'instruction

            switch (res_exe){
            case 0:
                WARNING("Erreur d'overflow dans l'éxécution du programme adresse: 0x%x\nLe programme continue\n", prev_addr);
            break;

            case 1:
                //fonctionnement normal ne fait rien
            break;

            case 2:
                printf("Breakpoint à l'adresse: 0x%x\nRend la main\n", prev_addr);
                return CMD_RVAL_OK;
            break;

            case 3:
                WARNING("Division par 0 à l'adresse: 0x%x\nAucune exécution\nLe programme continue\n", prev_addr);
            break;

            case 5:
                printf("Fin du programme suite à un break dans le fichier .s\n");
            break;

            case 10:
                printf("Fin du programme\n");
                return CMD_RVAL_OK;
            break;

            case 404:
                ERROR( "SegFault: Erreur entrainant une sortie du programme à l'adresse 0x%x\nRend la main à l'utilisateur\n", prev_addr);
                return CMD_RVAL_ERROR;
            break;
            
            default:
                ERROR( "SegFault: Erreur entrainant une sortie du programme à l'adresse 0x%x\nRend la main à l'utilisateur\n", prev_addr);
                return CMD_RVAL_ERROR;
            break;
            }
        }
    }while(interp->mips.reg.registres_usuel[34] < (interp->mips.memory->seg[segm1].start._32 + interp->mips.memory->seg[segm1].size._32));

    DEBUG("Fin d'éxécution du programme, adresse du pc:%x\n", interp->mips.reg.registres_usuel[34]);
    return CMD_RVAL_OK;
}



int execute_inst(interpreter interp, instruct_def *inst){
    DEBUG("Début d'éxécution de la fonction %s\n", inst[interp->dico].nom);

    if ( 0 == strcmp( inst[interp->dico].nom, "ADD" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_add(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "ADDI" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_addi(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "ADDIU" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_addiu(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "ADDU" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_addu(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "AND" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_and(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "ANDI" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_andi(interp);
    }

    
    if ( 0 == strcmp( inst[interp->dico].nom, "DIV" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_div(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "LUI" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_lui(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "MFHI" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_mfhi(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "MFLO" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_mflo(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "MULT" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_mult(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "NOP" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_nop();
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "OR" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_or(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "ORI" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_ori(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SEB" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_seb(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SLL" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sll(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SLT" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_slt(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SLTI" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_slti(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SLTIU" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sltiu(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SLTU" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sltu(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SRA" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sra(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SRL" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_srl(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SUB" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sub(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SUBU" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_subu(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SYSCALL" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_syscall(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "XOR" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_xor(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BREAK" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_break();
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BNE" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_bne(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BEQ" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_beq(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BGEZ" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_bgez(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BGTZ" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_bgtz(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BLEZ" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_blez(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "BLTZ" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_bltz(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "JR" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_jr(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "J" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_j(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "JAL" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_jal(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "JALR" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_jalr(interp, inst);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "LB" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_lb(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "LBU" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_lbu(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "LW" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_lw(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SB" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sb(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "SW" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_sw(interp);
    }

    if ( 0 == strcmp( inst[interp->dico].nom, "EXT" ) ) {
        interp->mips.reg.registres_usuel[34]+=4;//Maj du PC
        return exec_ext(interp);
    }

    return 404;
}

