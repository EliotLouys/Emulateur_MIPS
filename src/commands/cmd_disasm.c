/**
 * @file cmd_disasm.c
 * @author Serge Le Thanh
 * @date 30/09/2020
 * @brief Emulator command:disasm
 *
 * Emulator command: disasm
 */

#include "commands.h"
#include "bits.h"

cmd_rval_t cmd_disasm(interpreter interp, instruct_def *inst){

    char           *token = NULL;
    char 		   *endptr=NULL;
    uint32_t 		addr1, addr2;
    int 			j, z, i, flag1, flag2, segm1, segm2, indic1, indic2, jump, etiq;
    unsigned int    disp;
    string          err_loc; /* To log error location. */

    token = get_next_token( interp );

    if(interp->mips.memory == NULL){
	    MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Aucun fichier chargé %s\n", err_loc );
	return CMD_RVAL_ERROR;
    }

    else if (token == 0){
	    MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc ); 
	return CMD_RVAL_ERROR;
    }
    
    else if(token_type(token)==TOK_HEXA){
        addr1 = (uint32_t) strtol(token, &endptr, 16);
        token = get_next_token( interp );

        if(token==0){
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Commande inconnue %s\n", err_loc );
		    return CMD_RVAL_ERROR;
		}

        else if(token[0]==':'){
			DEBUG("lecture de :\n");
			token = get_next_token( interp );

            if (token == NULL){
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Manque borne max adresse\n");
	            return CMD_RVAL_ERROR;
            }
            else if(token_type(token) == TOK_HEXA){
                addr2 = (uint32_t) strtol(token, &endptr, 16);
            } 
            else{
	            MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Commande inconnue %s\nVeuillez rentrez un nombre hexadecimal\n", err_loc );
	            return CMD_RVAL_ERROR;
            }

            token = get_next_token( interp );
            if(token != NULL){
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Commande inconnue %s\n", err_loc );
	            return CMD_RVAL_ERROR;
            }
		}

        else if(token[0]=='+'){
			DEBUG("lecture de +\n");
			token = get_next_token( interp );
            
            if (token == NULL){
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Manque borne max adresse\n");
	            return CMD_RVAL_ERROR;
            }
            else if(token_type(token)==TOK_DEC){
                 addr2 = (uint32_t) strtol(token, &endptr, 10);
                 addr2 = addr2 + addr1;
            }
            else{
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Commande inconnue\nVeuillez rentrer un nombre décimal\n");
	            return CMD_RVAL_ERROR;
            }

            token = get_next_token( interp );
            if(token != NULL){
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Commande inconnue %s\n", err_loc );
	            return CMD_RVAL_ERROR;
            }
            
		}

        else{
	        MAKE_ERR_LOC( err_loc, interp ); 
	        ERROR( "Commande inconnue %s\n", err_loc );
	        return CMD_RVAL_ERROR;
        }

        real_addr_mem (interp, &segm1, &indic1, addr1, &flag1);
		real_addr_mem (interp, &segm2, &indic2, addr2, &flag2);
        DEBUG("falg1 = %d\nflag2 = %d", flag1, flag2);
        if(flag1!=0 || flag2!=0){
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "La borne de départ ou de fin n'existe pas ou n'est pas allouée\n");
			return CMD_RVAL_ERROR;
		}
        else if(segm1!=segm2){
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Les deux bornes ne sont pas dans le même segment\n");
			return CMD_RVAL_ERROR;
		}
        if(0!=strcmp(interp->mips.memory->seg[segm1].name, ".text")){
            MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "L'adresse de départ indiquée ne se trouve pas dans le segment .text\n");
			return CMD_RVAL_ERROR;
        }
        else if(0!=strcmp(interp->mips.memory->seg[segm2].name, ".text")){
            MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "L'adresse de fin indiquée ne se trouve pas dans le segment .text\n");
			return CMD_RVAL_ERROR;
        }

        i=0;

        for(i=0;i+indic1<=indic2;i+=4){
            disasm(interp, inst, segm1, indic1, i);
            //Gestion des étiquettes
            for(j=0;j<interp->mips.symtab.size;j++){
                if(interp->mips.symtab.sym[j].scnidx == segm1+1){
                    if(interp->mips.symtab.sym[j].addr._32 == i+indic1 && interp->mips.symtab.sym[j].type==notype){
                        DEBUG("adresse symtab:%d\n", interp->mips.symtab.sym[j].addr._32);
                        printf("%s:\n", interp->mips.symtab.sym[j].name);
                    }
                }
            }
            if(interp->dico == -1){
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Erreur de désassemblage\n");
	            return CMD_RVAL_ERROR;
            }

            else if(0 == strcmp(inst[interp->dico].type, "R")){
                printf("0x%x : : 0x%08x %s ", addr1+i, interp->word_value.asInt, inst[interp->dico].nom);
                for(z=0;z<inst[interp->dico].nb_param;z++){
                    if(0 == strcmp(inst[interp->dico].ordre[z], "rs")){
                    disp = interp->word_value.asR.rs;
                    DEBUG("Le champ rs vaut:%d\n", disp);
                    printf("%s ", interp->mips.reg.nom_registres[disp]);
                    }
                    else if(0 == strcmp(inst[interp->dico].ordre[z], "rt")){
                        disp = interp->word_value.asR.rt;
                        DEBUG("Le champ rt vaut:%d\n", disp);
                        printf("%s ", interp->mips.reg.nom_registres[disp]);
                    }
                    else if(0 == strcmp(inst[interp->dico].ordre[z], "rd")){
                        disp = interp->word_value.asR.rd;
                        DEBUG("Le champ rd vaut:%d\n", disp);
                        printf("%s ", interp->mips.reg.nom_registres[disp]);
                    }
                    else if(0 == strcmp(inst[interp->dico].ordre[z], "sa")){
                    disp = interp->word_value.asR.sa;
                    DEBUG("Le champ sa vaut:%d\n", disp);
                    printf("%d ", disp);
                    }
                    else{
                        MAKE_ERR_LOC( err_loc, interp ); 
	                    ERROR( "Erreur de désassemblage\n");
	                    return CMD_RVAL_ERROR;
                    }
                }
                printf("\n");
            }
            else if(0 == strcmp(inst[interp->dico].type, "J")){
                printf("0x%x : : 0x%08x %s ", addr1+i, interp->word_value.asInt, inst[interp->dico].nom);
                for(z=0;z<inst[interp->dico].nb_param;z++){
                    if(0 == strcmp(inst[interp->dico].ordre[z], "target")){
                        jump = i+indic1+4*interp->word_value.asJ.target;
                        DEBUG("%d \n",jump);
                        etiq = -1;
                        for(j=0;j<interp->mips.symtab.size;j++){
                            if(interp->mips.symtab.sym[j].scnidx == segm1+1){
                                if(interp->mips.symtab.sym[j].addr._32 == jump && interp->mips.symtab.sym[j].type==notype){
                                    printf("%s ", interp->mips.symtab.sym[j].name);
                                    etiq = 1;
                                }
                            }
                        }
                        if(etiq == -1){
                            printf("0x%x ", interp->word_value.asJ.target);
                        }
                    }
                    else{
                        MAKE_ERR_LOC( err_loc, interp ); 
	                    ERROR( "Erreur de désassemblage\n");
	                    return CMD_RVAL_ERROR;
                    }
                }
                printf("\n");        
            }
            else if(0 == strcmp(inst[interp->dico].type, "I")){
                printf("0x%x : : 0x%08x %s ", addr1+i, interp->word_value.asInt, inst[interp->dico].nom);
                for(z=0;z<inst[interp->dico].nb_param;z++){
                    if(0 == strcmp(inst[interp->dico].ordre[z], "rs")){
                        disp = interp->word_value.asI.rs;
                        DEBUG("Le champ rs vaut:%d\n", disp);
                        printf("%s ", interp->mips.reg.nom_registres[disp]);
                    }
                    else if(0 == strcmp(inst[interp->dico].ordre[z], "rt")){
                        disp = interp->word_value.asI.rt;
                        DEBUG("Le champ rt vaut:%d\n", disp);
                        printf("%s ", interp->mips.reg.nom_registres[disp]);
                    }
                    else if((0 == strcmp(inst[interp->dico].ordre[z], "immediate")) || (0 == strcmp(inst[interp->dico].ordre[z], "offset")) ){
                        printf("0x%x ", interp->word_value.asI.immediate);
                    }
                    else if(0 == strcmp(inst[interp->dico].ordre[z], "base")){
                        disp = interp->word_value.asI.rs;
                        DEBUG("Le champ rs vaut:%d\n", disp);
                        printf("(%s) ", interp->mips.reg.nom_registres[disp]);
                    }
                    else{
                        MAKE_ERR_LOC( err_loc, interp ); 
	                    ERROR( "Erreur de désassemblage\n");
	                    return CMD_RVAL_ERROR;
                    }
                }
                printf("\n");         
            }
            else{
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Type inconnu\n");
	            return -1;
            }
        }
        return CMD_RVAL_OK;
    }

    else{
	    MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Commande inconnue %s\n", err_loc );
	    return CMD_RVAL_ERROR;
    }

    return CMD_RVAL_ERROR;
}



 void disasm(interpreter interp, instruct_def *inst, int segm1, int indic1, int i){

     int j;

    interp->word_value.asCharArray[0] = interp->mips.memory->seg[segm1].content[indic1+i] ;
    interp->word_value.asCharArray[1] = interp->mips.memory->seg[segm1].content[indic1+i+1] ;
    interp->word_value.asCharArray[2] = interp->mips.memory->seg[segm1].content[indic1+i+2] ;
    interp->word_value.asCharArray[3] = interp->mips.memory->seg[segm1].content[indic1+i+3] ;
    DEBUG("1er octet:%d\n", interp->word_value.asCharArray[0]);
    DEBUG("2eme octet:%d\n", interp->word_value.asCharArray[1]);
    DEBUG("3eme octet:%d\n", interp->word_value.asCharArray[2]);
    DEBUG("4eme octet:%d\n", interp->word_value.asCharArray[3]);
    interp->word_value.asInt = (interp->word_value.asCharArray[0]<<24)|(interp->word_value.asCharArray[1]<<16)| (interp->word_value.asCharArray[2]<<8)|interp->word_value.asCharArray[3];
    DEBUG("Affichage du mot complet %x\n", interp->word_value.asInt);
    DEBUG("Début de recherche dans le dico\n");
    interp->dico = -1;
    for(j=0;j<NB_FUNC;j++){
        if(inst[j].signature == (interp->word_value.asInt & inst[j].masque)){
            DEBUG("Fonction trouvée\n");
            interp->dico = j;
        }
    }
 }