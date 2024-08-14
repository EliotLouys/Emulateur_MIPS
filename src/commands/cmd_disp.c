/**
 * @file cmd_disp.c
 * @author Serge Le Thanh 
 * @date 16/09/2020
 * @brief Emulator command: disp.
 *
 * Emulator command: disp. 
 */

#include "commands.h"


cmd_rval_t cmd_disp( interpreter interp ) { 

char           *token = NULL;
char 		   *endptr=NULL;
uint32_t 		addr1, addr2;
unsigned int    hexval;
int 			lr, i, flag1, flag2, num_reg, verif_reg=0, counta=0, segm1, segm2, indic1, indic2, disp;
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
	

else if ( 0 == strcmp( token, "mem" ) ){
	token = get_next_token( interp );
	
	if (token == 0){
	MAKE_ERR_LOC( err_loc, interp ); 
	ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	token, CMD_NAME, err_loc ); 
	return CMD_RVAL_ERROR;
	}
	
	else if ( 0 == strcmp( token, "map" ) ){
		token = get_next_token( interp );
		if(token == 0){
			print_mem( interp->mips.memory);
			return CMD_RVAL_OK;
		}
		else{
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Commande inconnue %s\n", err_loc );
			return CMD_RVAL_ERROR;
		}
	}
	
	else{
		DEBUG("Lecture d'une adresse\n");
		switch ( token_type( token ) ){
		case TOK_HEXA:
			sscanf( token,"%x", &hexval ); /* We know we have a hex, so scan it! */
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
			}
			else{
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Commande inconnue %s\n", err_loc );
				return CMD_RVAL_ERROR;
			}
			
			if(token==0){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Manque borne max plage mémoire %s\n", err_loc );
				return CMD_RVAL_ERROR;
			}
		
			switch ( token_type( token ) ){
			case TOK_HEXA:
				addr2 = (uint32_t) strtol(token, &endptr, 16);
				real_addr_mem (interp, &segm1, &indic1, addr1, &flag1);
				real_addr_mem (interp, &segm2, &indic2, addr2, &flag2);

				if(flag1!=0 || flag2!=0){
					MAKE_ERR_LOC( err_loc, interp ); 
					ERROR( "La borne de départ ou de fin n'existe pas ou n'est pas allouée\n");
					return CMD_RVAL_ERROR;
				}
	
				token = get_next_token( interp );
				if(token!=0){
					MAKE_ERR_LOC( err_loc, interp ); 
					ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
							token, CMD_NAME, err_loc );
					return CMD_RVAL_ERROR;
				}
				
				else{
					fprintf( stdout, "0x%08x\t", hexval); 
					i = 0;
					lr = 0;
					disp = 1;
					while(disp!=0){
						if(indic1+i>=interp->mips.memory->seg[segm1].size._32){
							segm1++;
							indic1 = 0;
							i=0;
							printf("\nChangement de segment: passage de %s à %s\n",interp->mips.memory->seg[segm1-1].name, interp->mips.memory->seg[segm1].name);
						}
						if(segm1>=segm2 && indic1+i>=indic2){
							disp = 0;
						}
						if((lr%16)==0 && lr!=0){
							printf("\n");
							fprintf( stdout, "0x%08x\t", interp->mips.memory->seg[segm1].start._32+indic1+i);
							lr =0;
						}
						printf("%02x\t", interp->mips.memory->seg[segm1].content[indic1+i]);
						lr++;
						i++;
					}
		
					printf("\n");
					return CMD_RVAL_OK;
				}
			break;
		
			default :
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
					token, CMD_NAME, err_loc ); 
      
				return CMD_RVAL_ERROR; 
			}
		break;
	
		default :
		MAKE_ERR_LOC( err_loc, interp ); 
		ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
			token, CMD_NAME, err_loc ); 
      
			return CMD_RVAL_ERROR; 
		
		}
	}
}

else if ( 0 == strcmp( token, "reg" ) ){
	DEBUG("Affichage des registres\n");
	token = get_next_token( interp );
	
	if(token==0){
		MAKE_ERR_LOC( err_loc, interp ); 
		ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
			token, CMD_NAME, err_loc );
		return CMD_RVAL_ERROR;
		
	}	
	
	else if ( 0 == strcmp( token, "all" ) ){
		token = get_next_token( interp );
		if(token!=0){
		MAKE_ERR_LOC( err_loc, interp ); 
		ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
			token, CMD_NAME, err_loc );
		return CMD_RVAL_ERROR;
		
		}
		else{
			for(i=0;i<35;i++){
				if((i%4)==0 || i==32){
					printf("\n");
				}
				printf("%s\t:\t%08x\t",interp->mips.reg.nom_registres[i],interp->mips.reg.registres_usuel[i]);
				if(i==34){
					printf("\n");
				}
			}
			return CMD_RVAL_OK;
		}
		
	}
	
	else if(token[0]=='$'){
		while(token!=0){
			if(token[0]!='$'){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
			}
			for(i=0;i<35;i++){
				if(0==strcmp(token,interp->mips.reg.nom_registres[i]) || 0==strcmp(token,interp->mips.reg.num_registres[i])){
				verif_reg = 1;
				num_reg = i;
				}
			}
			
			if(verif_reg){
				printf("%s\t:\t%08x\t",interp->mips.reg.nom_registres[num_reg],interp->mips.reg.registres_usuel[num_reg]);
				verif_reg = 0;
				counta++;
				if(counta==4){
					printf("\n");
					counta=0;
				}
			}
			else{
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
				}
			
			token = get_next_token( interp );
		}
		printf("\n");
		return CMD_RVAL_OK;
	}
	
	else{
		MAKE_ERR_LOC( err_loc, interp ); 
		ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
			token, CMD_NAME, err_loc );
		return CMD_RVAL_ERROR;
	}
}

else{
	MAKE_ERR_LOC( err_loc, interp ); 
	ERROR( "Commande inconnue %s\n", err_loc );
	return CMD_RVAL_ERROR;
}
}
