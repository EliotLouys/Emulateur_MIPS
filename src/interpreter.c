
/**
 * @file interpreter.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Aug 19 00:06:43 2020
 * @brief Interpreter definitions.
 *
 * Interpreter definitions.
 */

#include "interpreter.h"


/**
 * @brief Find and return the next token string in the input line currently under scrutiny.
 * This function uses a variable internal to the interpreter state to manage the first tokenization
 * steps.
 * @param interp Pointer to interpreter state.
 * @return Pointer to the next token string, or NULL in case no more tokens are available.
 */
char *get_next_token( interpreter interp ) {

  char  *token = NULL;
  char  *delim = " \t\n";

  if ( interp->first_token == 0 ) {
    token = strtok_r( interp->input, delim, &(interp->from) );
    interp->first_token = 1;
  }
  else {
    token = strtok_r( NULL, delim, &(interp->from) );
  }

  if ( NULL == token ) {
    interp->first_token = 0;
  }

  return token;
}

/**
 * @brief Check whether input token string contains a hex number.
 * @attention This method is not fool-proof and will not work in the general case.
 * Check what happens with 0x567ZRT for example!
 * A better approach would use the strtol(3) function (and friends).
 * @todo Write a (much!) robust version of this function.
 * @param token The token string we want to check whether it contains a hex number.
 * @return 0 if token is non-hex, 1 otherwise.
 */
int is_hexa( char *token ) {
  /*int i;

  return
    token != NULL       &&
    strlen( token ) > 2 &&
    token[0]=='0'       &&
    token[1]=='x'       &&
    sscanf( token, "%x", &i ) == 1;*/

	//long val;
	char *endptr=NULL;
	DEBUG("Utilisation de la fonction is hexa\n");
	if(token[0]=='0' && token[1]=='x'){
		strtol(token, &endptr, 16);
		if(*endptr==NULL){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

int is_octal( char *token ) {

	char *endptr=NULL;
	DEBUG("Utilisation de la fonction is octal\n");
	if(token[0]=='0'){
		strtol(token, &endptr, 8);
		if(*endptr==NULL){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

int is_dec( char *token ) {

	char *endptr=NULL;
	DEBUG("Utilisation de la fonction is dec\n");
	strtol(token, &endptr, 10);
	if(*endptr==NULL){
		return 1;
	}
	else{
		return 0;
	}
}

/**
 * @brief Returns the token type, you have to complete this function with missing topen types!
 * @attention Since a token is always a string, we always return @ref TOK_STRING in last resort.
 * @param token Input token string to be analyzed. Defaults to @ref TOK_STRING.
 * @return Token type.
 *
 */
token_t token_type( char *token ) {

int state=0, i=1;
  DEBUG("Recherche du type du token\n");
	while(i!=0){
	switch(state){
	  case 0:
	  DEBUG("Recherche du type du token au stade 0\n");
	  if(token[0] == '0'){
		  state = 1;
		  DEBUG("Passage au stade 1 de la recherche\n");
	  }
	  else if (isdigit(token[0])){
		  state = 3;
		  DEBUG("Passage au stade 3 de la recherche\n");
	  }
	  else if(token[0] == '-'){
		  state = 4;
		  DEBUG("Passage au stade 4 de la recherche\n");
	  }
    else if(token[0] == '$'){
      i=0;
      return TOK_REG;
    }
	  else{
		  i=0;
		  return TOK_STRING;
	  }
	  break;

	  case 1:
	  DEBUG("Recherche du type du token au stade 1\n");
	  if(token[0]=='0' && token[1]=='x'){
		  state = 2;
		  DEBUG("Passage au stade 2 de la recherche\n");
	  }
	  else if(token[0]=='0' && token[1]=='\0'){
		  i=0;
		  return TOK_DEC;
	  }
	  else if(is_octal(token)){
		  i=0;
		  return TOK_OCTA;
	  }
	  else{
		i=0;
		return TOK_STRING;;
	  }
	  break;

	  case 2:
	  DEBUG("Recherche du type du token au stade 2\n");
	  if ( is_hexa( token ) ) {
		i=0;
		return TOK_HEXA;
	  }
	  else{
		  i=0;
		  return TOK_STRING;
	  }
	  break;

	  case 3:
	  DEBUG("Recherche du type du token au stade 3\n");
	  if(is_dec(token)){
		i=0;
		return TOK_DEC;
	  }
	  else{
		  i=0;
		  return TOK_STRING;
	  }
	  break;

	  case 4:
	  DEBUG("Recherche du type du token au stade 4\n");
	  if(token[1] == '0'){
		  state = 1;
		  DEBUG("Passage au stade 1 de la recherche\n");
	  }
	  else if (isdigit(token[1])){
		  state = 3;
		  DEBUG("Passage au stade 3 de la recherche\n");
	  }
	  else{
		  return TOK_STRING;
	  }
	  break;

	  default:
		i=0;
		return TOK_STRING;

	}
	}

  return TOK_STRING;
}

/**
 * @brief Interpreter allocation and init.
 * @return Pointer to a newly allocated interpreter state, initialized for @ref INTERACTIVE mode.
 */
interpreter interp_new( void ) {
  interpreter interp = calloc( 1, sizeof( *interp ) );

  if ( NULL == interp ) {
    ERROR("Unable to allocate memory for new interpreter");
    return NULL;
  }

  interp->mode        = INTERACTIVE;
  interp->fp          = stdin;
  interp->save_fp     = NULL;
  interp->script_file = NULL;
  interp->script_line = 0;
  interp->from        = NULL;
  interp->first_token = '\0';

  return interp;
}

/**
 * @brief Interpreter deallocation.
 * @param interp Pointer to interpreter state that should be freed.
 */
void interp_del( interpreter interp ) {

  if ( stdin != interp->fp ) {
    fclose( interp->fp );
  }
  del_machine( interp->mips );
  free( interp );
  return;
}



/**
 * @brief Find the real address of the mips.
 * @param interp Pointer to interpreter state.
 * @param segm Pointer to address of the segment.
 * @param indic Pointer to address of the content.
 * @param virtual_addr Virtual address of the mips.
 * @param flag Used to indicate of the virtual address exist or not.
 */
void real_addr_mem (interpreter interp, int *segm, int *indic, uint32_t virtual_addr, int *flag){

	int i;
	DEBUG("adresse virtuelle voulue: %x\n", virtual_addr);

	
	*flag = -1;
	for(i=0;i<interp->mips.memory->nseg;i++){
		DEBUG("adresse de départ du segment:0x%08x\n", interp->mips.memory->seg[i].start._32);
		DEBUG("Taille du segment: %d\n", interp->mips.memory->seg[i].size._32);
		DEBUG("borne max de l'adresse virtuelle du segment en cours 0x%08x\n", interp->mips.memory->seg[i].start._32 + interp->mips.memory->seg[i].size._32) ;
		if((interp->mips.memory->seg[i].start._32 <= virtual_addr) && 
		(interp->mips.memory->seg[i].start._32 + interp->mips.memory->seg[i].size._32 >= virtual_addr)){
			*segm = i;
			*indic = virtual_addr - interp->mips.memory->seg[i].start._32;
			*flag  = 0;
			if(*indic>interp->mips.memory->seg[i].size._32){
				DEBUG("Taille trop gradne\n");
				*flag = -1;
			}
		}	
	}

}


/**
 * @brief Used to convert from bigEndian to littleEndian.
 * @param p1 Pointer to base word in littleEndian.
 * @param p2 Pointer to new word in bigEndian.
 */
void swap (char *p1, char *p2) {
    char tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

/**
 * @brief Used to read from dico the disasm instruction def
 */
instruct_def *lecture_dico(){
	FILE *f;
	instruct_def *inst;
	int nb_func, i, j;
	char *hex_temp;
	char *end_ptr = NULL;

	DEBUG("entrée dans la lecture du dico\n");
	f = fopen("dico.txt", "r");
	DEBUG("Lecture du nombrre de fonction dans le dico\n");
	fscanf(f, "%d", &nb_func);
	DEBUG("Nombre de fonction dans le dico: %d\n", nb_func);

	DEBUG("Allocation de case pour le tableau d'instruction\n");
	inst = calloc(nb_func, sizeof(instruct_def));

	DEBUG("Début de lecture des instructions\n");
	for(i=0;i<nb_func;i++){
		fscanf(f, "%s",inst[i].nom );
		fscanf(f, "%s", &hex_temp);
		inst[i].signature = strtoul(&hex_temp, &end_ptr, 16);
		fscanf(f, "%s", &hex_temp);
		inst[i].masque = strtoul(&hex_temp, &end_ptr, 16);
		fscanf(f, "%s", inst[i].type);
		fscanf(f, "%d", &inst[i].nb_param);
		for(j=0;j<inst[i].nb_param;j++){
			fscanf(f, "%s", inst[i].ordre[j]);
		}
	}
	DEBUG("Fin de lecture du dico\n");
	fclose(f);
	DEBUG("Fermeture du fichier text\n");

	return inst;
}

void print_dico(instruct_def *inst){
	int i, j;

	for(i=0;i<NB_FUNC;i++){
		DEBUG("Nom de la fonction: %s\n", inst[i].nom);
		DEBUG("Signature de la fonction: %08x\n", inst[i].signature);
		DEBUG("Masque de la fonction: %08x\n", inst[i].masque);
		DEBUG("Type de la fonction: %s\n", inst[i].type);
		DEBUG("Nombre de param de la fonction: %d\n", inst[i].nb_param);
		for(j=0;j<inst[i].nb_param;j++){
			DEBUG("%s\n", inst[i].ordre[j]);
		}
	}
}


void del_dico(instruct_def *inst){
	free(inst);
	return;
}