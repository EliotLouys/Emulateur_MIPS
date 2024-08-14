
#include "commands.h"
#include "bits.h"


cmd_rval_t cmd_set(interpreter interp){

char           *token = NULL;
char 		   *token_interm=NULL;
char 		   *endptr=NULL;
byte 			mem_val, m0, m1, m2, m3;
long long		mem_val_cmp;
uint32_t 		verif_reg=0, num_reg, mem_val_w;
int64_t 		val1;
int 			i, flag, segm, indic;
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
	DEBUG("Paramétrage de la mémoire\n");
	token = get_next_token( interp );
	
	if (token == 0){
	MAKE_ERR_LOC( err_loc, interp ); 
	ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	token, CMD_NAME, err_loc ); 
	return CMD_RVAL_ERROR;
	}
	
	DEBUG("Lecture de l'adresse\n");
	switch ( token_type( token ) ){
		case TOK_HEXA:
			DEBUG("Addresse lue:%s\n", token);
			DEBUG("Recherche de l'adresse correspondante\n");
      		val1 = (uint32_t) strtol(token, &endptr, 16);
      		real_addr_mem (interp, &segm, &indic, val1, &flag);

			if(flag!=0){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Adresse virtuelle non existante\n"); 
				return CMD_RVAL_ERROR;
			}
			
			token = get_next_token( interp );
			DEBUG("Lecture du type byte ou word\n");
			if (token == 0){
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
			token, CMD_NAME, err_loc ); 
			return CMD_RVAL_ERROR;
			}
			
			else if (0 == strcmp(token, "byte")){
				token = get_next_token( interp );
				DEBUG("Lecture de la valeur à écrire\n");
	
				if (token == 0){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
				}
				else{
					DEBUG("Affichage de la valeur lue:%s\n", token);
					if(token[0]=='-'){
					token_interm = token;
					token = get_next_token( interp );
					token = strcat(token_interm, token);
					DEBUG("Affichage de la valeur lue:%s\n", token);
					}
					
					token_interm = get_next_token( interp );
					if (token_interm != 0){
						MAKE_ERR_LOC( err_loc, interp ); 
						ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
						token_interm, CMD_NAME, err_loc ); 
						return CMD_RVAL_ERROR;
					}
				
					switch(token_type(token)){
					case TOK_HEXA:
						mem_val_cmp = strtol(token,&endptr,16);
      					mem_val = (byte) mem_val_cmp;
						if(mem_val_cmp>255 || mem_val_cmp<-127){
        					MAKE_ERR_LOC( err_loc, interp ); 
	      					ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      					token, CMD_NAME, err_loc ); 
	      					return CMD_RVAL_ERROR;
      					}
						else{
							DEBUG("Affectation de la valeur dans la case mémoire\n");
							interp->mips.memory->seg[segm].content[indic] = mem_val;
							DEBUG("Valeur stockée dans la case mémoire: %d\n", interp->mips.memory->seg[segm].content[indic]);
							return CMD_RVAL_OK;
						}
					break;
		
					case TOK_OCTA:
						mem_val_cmp = strtol(token,&endptr,8);
      					mem_val = (byte) mem_val_cmp;
						if(mem_val_cmp>255 || mem_val_cmp<-127){
        					MAKE_ERR_LOC( err_loc, interp ); 
	      					ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      					token, CMD_NAME, err_loc ); 
	      					return CMD_RVAL_ERROR;
      					}
						else{
							DEBUG("Affectation de la valeur dans la case mémoire\n");
							interp->mips.memory->seg[segm].content[indic] = mem_val;
							DEBUG("Valeur stockée dans la case mémoire: %d\n", interp->mips.memory->seg[segm].content[indic]);
							return CMD_RVAL_OK;
						}
					break;
		
					case TOK_DEC:
						mem_val_cmp = strtol(token,&endptr,10);
      					mem_val = (byte) mem_val_cmp;
						if(mem_val_cmp>255 || mem_val_cmp<-127){
        					MAKE_ERR_LOC( err_loc, interp ); 
	      					ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      					token, CMD_NAME, err_loc ); 
	      					return CMD_RVAL_ERROR;
      					}
						else{
							DEBUG("Affectation de la valeur dans la case mémoire\n");
							interp->mips.memory->seg[segm].content[indic] = mem_val;
							DEBUG("Valeur stockée dans la case mémoire: %d\n", interp->mips.memory->seg[segm].content[indic]);
							return CMD_RVAL_OK;
						}
					break;
		
					default:
						MAKE_ERR_LOC( err_loc, interp ); 
						ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
						token, CMD_NAME, err_loc ); 
					return CMD_RVAL_ERROR;
					}
				}
			}
			else if (0 == strcmp(token, "word")){
				token = get_next_token( interp );
				DEBUG("Lecture de la valeur à écrire\n");
	
				if (token == 0){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
				}
				else{
					DEBUG("Affichage de la valeur lue:%s\n", token);
					if(token[0]=='-'){
					token_interm = token;
					token = get_next_token( interp );
					token = strcat(token_interm, token);
					DEBUG("Affichage de la valeur lue:%s\n", token);
					}
					
					token_interm = get_next_token( interp );
					if (token_interm != 0){
						MAKE_ERR_LOC( err_loc, interp ); 
						ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
						token_interm, CMD_NAME, err_loc ); 
						return CMD_RVAL_ERROR;
					}
				
					switch(token_type(token)){
					case TOK_HEXA:
						mem_val_cmp = strtoll(token,&endptr,16);
      					DEBUG("%lld\n", mem_val_cmp);
      					mem_val_w = (uint32_t) mem_val_cmp;
						DEBUG("%x\n", mem_val_w);
						m0 = mem_val_w & 0xFF;
						m1 = (mem_val_w & 0xFF00)>>8;
						m2 = (mem_val_w & 0xFF0000)>>16;
						m3 = (mem_val_w & 0xFF000000)>>24;
						DEBUG("%x\n", m0);
						DEBUG("%x\n", m1);
						DEBUG("%x\n", m2);
						DEBUG("%x\n", m3);
						if(mem_val_cmp>4294967295 || mem_val_cmp< -268435456){
       						 MAKE_ERR_LOC( err_loc, interp ); 
	      					ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      					token, CMD_NAME, err_loc ); 
	      					return CMD_RVAL_ERROR;
      					}
						else{
							DEBUG("Affectation de la valeur dans la case mémoire\n");
							interp->mips.memory->seg[segm].content[indic] = m3;
							interp->mips.memory->seg[segm].content[indic+1] = m2;
							interp->mips.memory->seg[segm].content[indic+2] = m1;
							interp->mips.memory->seg[segm].content[indic+3] = m0;
							DEBUG("Valeur stockée dans la case mémoire: %d %d %d %d\n", interp->mips.memory->seg[segm].content[indic],
							interp->mips.memory->seg[segm].content[indic+1], interp->mips.memory->seg[segm].content[indic+2],
							interp->mips.memory->seg[segm].content[indic+3]);
							return CMD_RVAL_OK;
						}
					break;
		
					case TOK_OCTA:
						mem_val_cmp = strtoll(token,&endptr,8);
      					DEBUG("%lld\n", mem_val_cmp);
      					mem_val_w = (uint32_t) mem_val_cmp;
      					m0 = mem_val_w & 0xFF;
						m1 = (mem_val_w & 0xFF00)>>8;
						m2 = (mem_val_w & 0xFF0000)>>16;
						m3 = (mem_val_w & 0xFF000000)>>24;
						if(mem_val_cmp>4294967295 || mem_val_cmp< -268435456){
       						 MAKE_ERR_LOC( err_loc, interp ); 
	      					ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      					token, CMD_NAME, err_loc ); 
	      					return CMD_RVAL_ERROR;
      					}
						else{
							DEBUG("Affectation de la valeur dans la case mémoire\n");
							interp->mips.memory->seg[segm].content[indic] = m3;
							interp->mips.memory->seg[segm].content[indic+1] = m2;
							interp->mips.memory->seg[segm].content[indic+2] = m1;
							interp->mips.memory->seg[segm].content[indic+3] = m0;
							DEBUG("Valeur stockée dans la case mémoire: %d %d %d %d\n", interp->mips.memory->seg[segm].content[indic],
							interp->mips.memory->seg[segm].content[indic+1], interp->mips.memory->seg[segm].content[indic+2],
							interp->mips.memory->seg[segm].content[indic+3]);
							return CMD_RVAL_OK;
						}
					break;
		
					case TOK_DEC:
						mem_val_cmp = strtoll(token,&endptr,10);
      					DEBUG("%lld\n", mem_val_cmp);
      					mem_val_w = (uint32_t) mem_val_cmp;
      					m0 = mem_val_w & 0xFF;
						m1 = (mem_val_w & 0xFF00)>>8;
						m2 = (mem_val_w & 0xFF0000)>>16;
						m3 = (mem_val_w & 0xFF000000)>>24;
						if(mem_val_cmp>4294967295 || mem_val_cmp< -268435456){
       						 MAKE_ERR_LOC( err_loc, interp ); 
	      					ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      					token, CMD_NAME, err_loc ); 
	      					return CMD_RVAL_ERROR;
      					}
						else{
							DEBUG("Affectation de la valeur dans la case mémoire\n");
							interp->mips.memory->seg[segm].content[indic] = m3;
							interp->mips.memory->seg[segm].content[indic+1] = m2;
							interp->mips.memory->seg[segm].content[indic+2] = m1;
							interp->mips.memory->seg[segm].content[indic+3] = m0;
							DEBUG("Valeur stockée dans la case mémoire: %d %d %d %d\n", interp->mips.memory->seg[segm].content[indic],
							interp->mips.memory->seg[segm].content[indic+1], interp->mips.memory->seg[segm].content[indic+2],
							interp->mips.memory->seg[segm].content[indic+3]);
							return CMD_RVAL_OK;
						}
					break;
		
					default:
						MAKE_ERR_LOC( err_loc, interp ); 
						ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
						token, CMD_NAME, err_loc ); 
					return CMD_RVAL_ERROR;
					}
				}
			}
			else {
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
			}
			
		break;
		
		default:
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
			token, CMD_NAME, err_loc ); 
			return CMD_RVAL_ERROR;
	}
}

else if ( 0 == strcmp( token, "reg" ) ){
	DEBUG("Paramétrage des registres\n");
	token = get_next_token( interp );
	while(token != NULL){
	
	if (token == 0){
	MAKE_ERR_LOC( err_loc, interp ); 
	ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	token, CMD_NAME, err_loc ); 
	return CMD_RVAL_ERROR;
	}
	
	for(i=0;i<35;i++){
		DEBUG("Recherche du registre correspondant\n");
		if(0==strcmp(token,interp->mips.reg.nom_registres[i]) || 0==strcmp(token,interp->mips.reg.num_registres[i])){
			verif_reg = 1;
			num_reg = i;
		}
	}
	if(verif_reg){
		DEBUG("Registre trouvé\n");
		token = get_next_token( interp );
	}
	else{
		MAKE_ERR_LOC( err_loc, interp ); 
		ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
		token, CMD_NAME, err_loc ); 
		return CMD_RVAL_ERROR;
	}
	DEBUG("Affichage de la valeur lue:%s\n", token);
	if(token[0]=='-'){
		token_interm = token;
		token = get_next_token( interp );
		token = strcat(token_interm, token);
		DEBUG("Affichage de la valeur lue:%s\n", token);
	}
	switch(token_type(token)){
		case TOK_HEXA:
			val1 = strtol(token, &endptr, 16);
			DEBUG("La valeur lue est %ld\n", val1);
			if(val1<-2147483648 || val1>4294967295){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).Valeur trop élevée\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
			}
			else{
				DEBUG("Affectation de la valeur dans le registre\n");
				interp->mips.reg.registres_usuel[num_reg] = (uint32_t) val1;
				DEBUG("Valeur stockée dans le registre: %d\n", interp->mips.reg.registres_usuel[num_reg]);
				//return CMD_RVAL_OK;
			}
		break;
		
		case TOK_OCTA:
			val1 = strtol(token, &endptr, 8);
			if(val1<-2147483648 || val1>4294967295){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).Valeur trop élevée\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
			}
			else{
				DEBUG("Affectation de la valeur dans le registre\n");
				interp->mips.reg.registres_usuel[num_reg] = (uint32_t) val1;
				DEBUG("Valeur stockée dans le registre: %d\n", interp->mips.reg.registres_usuel[num_reg]);
				//return CMD_RVAL_OK;
			}
		break;
		
		case TOK_DEC:
			DEBUG("La valeur lue est un decimal\n");
			val1 = strtol(token, &endptr, 10);
			DEBUG("La valeur lue est %ld\n", val1);
			if(val1<-2147483648 || val1>4294967295){
				MAKE_ERR_LOC( err_loc, interp ); 
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).Valeur trop élevée\n",
				token, CMD_NAME, err_loc ); 
				return CMD_RVAL_ERROR;
			}
			else{
				DEBUG("Affectation de la valeur dans le registre\n");
				interp->mips.reg.registres_usuel[num_reg] = (uint32_t) val1;
				DEBUG("Valeur stockée dans le registre: %d\n", interp->mips.reg.registres_usuel[num_reg]);
				//return CMD_RVAL_OK;
			}
		break;
		
		default:
		MAKE_ERR_LOC( err_loc, interp ); 
		ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
		token, CMD_NAME, err_loc ); 
		return CMD_RVAL_ERROR;
	}
	token = get_next_token( interp );
	}
	return CMD_RVAL_OK;
	
}	
else{
	MAKE_ERR_LOC( err_loc, interp ); 
	ERROR( "Commande inconnue %s\n", err_loc );
	return CMD_RVAL_ERROR;

}
}


