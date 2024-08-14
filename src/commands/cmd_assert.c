/**
 * @file cmd_assert.c
 * @author Eliot Louys
 * @date 20/09/2020
 * @brief Emulator command:assert
 *
 * Emulator command: assert
 */

#include "commands.h"
#include "bits.h"


cmd_rval_t cmd_assert(interpreter interp) {

  char           *token = NULL;
  char           *token_interm = NULL;
  char 		       *endptr=NULL;
  int             verif_reg=0, i, num_reg, flag, segm, indic;
  uint32_t 		    address, m0, m1, m2, m3;
  long long       mem_val_cmp;
  uint32_t        mem_val_w;
  byte            mem_val;
  string          err_loc; /* To log error location. */


  token=get_next_token(interp);

/**
  *This checks if a Machine is running and if a file is loaded in it
  **/

  if(interp->mips.memory == NULL){
	  MAKE_ERR_LOC( err_loc, interp );
	  ERROR( "Aucun fichier chargé %s\n", err_loc );
	  return CMD_RVAL_ERROR;
  }

/**
  *A checkout if there is a next argument to the function
  **/
  else if (token == 0){
	  MAKE_ERR_LOC( err_loc, interp );
	  ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	  token, CMD_NAME, err_loc );
	  return CMD_RVAL_ERROR;
  }

/**
  *From now, the program is dealing with the case the value is a "word" type
  *Then it checks again for a next argument
  **/
  else if (0 == strcmp(token,"word")){
    DEBUG("Lecture d'un word\n");
    token=get_next_token(interp);

    if (token == 0){
	    MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    }
/**
  *Negative case are dealt here
  **/
    if(token[0]=='-'){
			token_interm = token;
			token = get_next_token( interp );
			token = strcat(token_interm, token);
			DEBUG("Affichage de la valeur lue:%s\n", token);
		}

/**
  *Verifying if the value is an hexadecimal number, an octadecimal or a decimal one
  *Then checking if the value is possible and not overflowing for 32bits
  **/
    switch (token_type(token))
    {
    case TOK_HEXA:
      mem_val_cmp = strtoll(token,&endptr,16);
      DEBUG("%lld\n", mem_val_cmp);
      if(mem_val_cmp>4294967295 || mem_val_cmp< -268435456){
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      token, CMD_NAME, err_loc );
	      return CMD_RVAL_ERROR;
      }
      mem_val_w = (uint32_t) mem_val_cmp;
      m0 = mem_val_w & 0xFF;
			m1 = (mem_val_w & 0xFF00)>>8;
			m2 = (mem_val_w & 0xFF0000)>>16;
			m3 = (mem_val_w & 0xFF000000)>>24;
    break;

    case TOK_DEC:
      mem_val_cmp = strtoll(token,&endptr,10);
      if(mem_val_cmp>4294967295 || mem_val_cmp<-268435456){
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      token, CMD_NAME, err_loc );
	      return CMD_RVAL_ERROR;
      }
      mem_val_w = (uint32_t) mem_val_cmp;
      m0 = mem_val_w & 0xFF;
			m1 = (mem_val_w & 0xFF00)>>8;
			m2 = (mem_val_w & 0xFF0000)>>16;
			m3 = (mem_val_w & 0xFF000000)>>24;
    break;

    case TOK_OCTA:
      mem_val_cmp = strtoll(token,&endptr,8);
      if(mem_val_cmp>4294967295 || mem_val_cmp<-268435456){
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      token, CMD_NAME, err_loc );
	      return CMD_RVAL_ERROR;
      }
      mem_val_w = (uint32_t) mem_val_cmp;
      m0 = mem_val_w & 0xFF;
			m1 = (mem_val_w & 0xFF00)>>8;
			m2 = (mem_val_w & 0xFF0000)>>16;
			m3 = (mem_val_w & 0xFF000000)>>24;
    break;

    default:
      MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    break;
    }

    DEBUG("Lecture d'un registre ou d'une adresse mémoire\n");
    token=get_next_token(interp);

/**
  *A checkout if there is a next argument to the function
  **/

    if (token == 0){
	    MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    }

/**
  *Here, checking if the program must write on a registre or on a memory adress
  *if it has the typology of a registre with a $, the programm checks if the registre
  *really exist in the mips.
  **/
    switch (token_type(token))
    {
    case TOK_REG:
    DEBUG("Recherche du registre correspondant\n");
      for(i=0;i<35;i++){
				if(0==strcmp(token,interp->mips.reg.nom_registres[i]) || 0==strcmp(token,interp->mips.reg.num_registres[i])){
          DEBUG("Registre trouvé\n");
				verif_reg = 1;
				num_reg = i;
				}
			}

/**
  *Checks if the value inside the register is the one given to the interpretor and return the appropriate response wether it's the good value or not.
  **/
			if(verif_reg){
        if(interp->mips.reg.registres_usuel[num_reg] == mem_val_w){
          printf("La valeur testée: 0x%x est la même que dans le registre %s :0x%x\n", mem_val_w, interp->mips.reg.nom_registres[num_reg], interp->mips.reg.registres_usuel[num_reg]);
          return CMD_RVAL_OK;
        }
        else{
          printf("La valeur testée: 0x%x n'est  pas la même que dans le registre %s :0x%x\n", mem_val_w, interp->mips.reg.nom_registres[num_reg], interp->mips.reg.registres_usuel[num_reg]);
          return CMD_RVAL_ERROR;
        }
				verif_reg = 0;
			}
			else{
				MAKE_ERR_LOC( err_loc, interp );
				ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
				token, CMD_NAME, err_loc );
				return CMD_RVAL_ERROR;
				}
    break;

/**
  *Search for the adress given and then tests if the value is the one at this adress.
  *If not a good adress, it returns an error
  **/
    case TOK_HEXA:
      DEBUG("Recherche de l'adresse correspondante\n");
      address = (uint32_t) strtol(token, &endptr, 16);
      real_addr_mem (interp, &segm, &indic, address, &flag);
      if(flag == 0){
        if(interp->mips.memory->seg[segm].content[indic] == m3
        && interp->mips.memory->seg[segm].content[indic+1] == m2
        && interp->mips.memory->seg[segm].content[indic+2] == m1
        && interp->mips.memory->seg[segm].content[indic+3] == m0){
          printf("La valeur testée est la bonne\n");
          return CMD_RVAL_OK;
        }
        else{
          printf("La valeur testée n'est pas la bonne\n");
          return CMD_RVAL_ERROR;
        }
      }
      else{
        DEBUG("%d\n", flag);
        DEBUG("Adresse non existante\n");
        return CMD_RVAL_ERROR;
      }
    break;

    default:
      MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    break;
    }
  }

/**
  *From now, the program is dealing with the case the value is a "byte" type
  *Then it checks again for a next argument
  **/

  else if (0 == strcmp(token,"byte")){
    DEBUG("Lecture d'un byte\n");
    token=get_next_token(interp);

    if (token == 0){
	    MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    }

/**
  *Negative case are dealt here
  **/
    if(token[0]=='-'){
			token_interm = token;
			token = get_next_token( interp );
			token = strcat(token_interm, token);
			DEBUG("Affichage de la valeur lue:%s\n", token);
		}

/**
  *Same thing as earlier with the "word" except here, we are on 8 bit so the possible values
  *are a lot fewer.
  **/
    switch (token_type(token))
    {
    case TOK_HEXA:
      mem_val_cmp = strtol(token,&endptr,16);
      if(mem_val_cmp>255 || mem_val_cmp<-127){
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      token, CMD_NAME, err_loc );
	      return CMD_RVAL_ERROR;
      }
      mem_val = (byte) mem_val_cmp;
    break;

    case TOK_DEC:
      mem_val_cmp = strtol(token,&endptr,10);
      if(mem_val_cmp>255 || mem_val_cmp<-127){
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      token, CMD_NAME, err_loc );
	      return CMD_RVAL_ERROR;
      }
      mem_val = (byte) mem_val_cmp;
    break;

    case TOK_OCTA:
      mem_val_cmp = strtol(token,&endptr,8);
      if(mem_val_cmp>255 || mem_val_cmp<-127){
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Argument '%s' has invalid type for command '%s' (%s). Argument trop élevé pour le type\n",
	      token, CMD_NAME, err_loc );
	      return CMD_RVAL_ERROR;
      }
      mem_val = (byte) mem_val_cmp;
    break;

    default:
      MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    break;
    }

    DEBUG("Lecture d'une adresse\n");
    token=get_next_token(interp);

/**
  *A checkout if there is a next argument to the function
  **/

    if (token == 0){
	    MAKE_ERR_LOC( err_loc, interp );
	    ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	    token, CMD_NAME, err_loc );
	    return CMD_RVAL_ERROR;
    }

/**
  *Here, it can't be in a regsitre, so it only checks if the value is the one at the adress.
  *If the adress isn't good, it returns an error.
  **/
    if(token_type(token)==TOK_HEXA){
      address = (uint32_t) strtol(token, &endptr, 16);
      real_addr_mem (interp, &segm, &indic, address, &flag);
      if(flag==0){
        if(mem_val == interp->mips.memory->seg[segm].content[indic]){
          printf("La valeur testée: 0x%x est la même que dans la mémoire:0x%x\n", mem_val, interp->mips.memory->seg[segm].content[indic]);
        return CMD_RVAL_OK;
        }
        else{
          printf("La valeur testée: 0x%x n'est pas la même que celle dans la mémoire: 0x%x\n", mem_val, interp->mips.memory->seg[segm].content[indic]);
          return CMD_RVAL_ERROR;
        }
      }
      else{
        MAKE_ERR_LOC( err_loc, interp );
	      ERROR( "Adresse non existante\n");
	      return CMD_RVAL_ERROR;
      }
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
	  ERROR( "Argument '%s' has invalid type for command '%s' (%s).\n",
	  token, CMD_NAME, err_loc );
	  return CMD_RVAL_ERROR;
  }



}
