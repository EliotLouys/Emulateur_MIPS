/**
 * @file cmd_break.c
 * @author Serge Le Thanh
 * @date 02/11/2020
 * @brief Emulator command:break
 *
 * Emulator command: break
 */

#include "commands.h"
#include "bits.h"

cmd_rval_t cmd_break(interpreter interp){
    char           *token = NULL;
    char 		   *endptr=NULL;
    uint32_t 		addr1;
    int 			flag1, segm1, indic1;
    string          err_loc; /* To log error location. */

    token = get_next_token( interp );

    if(interp->mips.memory == NULL){
	    MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Aucun fichier chargé %s\n", err_loc );
	    return CMD_RVAL_ERROR;
    }
    else if(token == NULL){
        MAKE_ERR_LOC( err_loc, interp ); 
	    ERROR( "Erreur argument manquant\n");
	    return CMD_RVAL_ERROR;
    }

    else if(0 == strcmp(token, "add")){
        token = get_next_token( interp );
        if(token==0){
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Commande inconnue %s\n", err_loc );
		    return CMD_RVAL_ERROR;
		}

        while(token!=0){
            if(token_type(token)==TOK_HEXA){
                addr1 = (uint32_t) strtol(token, &endptr, 16);
                real_addr_mem (interp, &segm1, &indic1, addr1, &flag1);
                if(flag1!=0){
                    MAKE_ERR_LOC( err_loc, interp ); 
			        ERROR( "L'adresse indiquée n'est pas allouée\n");
			        return CMD_RVAL_ERROR;
                }
                else if(0!=strcmp(interp->mips.memory->seg[segm1].name, ".text")){
                    MAKE_ERR_LOC( err_loc, interp ); 
			        ERROR( "L'adresse indiquée ne se trouve pas dans le segment .text\n");
			        return CMD_RVAL_ERROR;
                }
                else{
                    add_break(interp, addr1);
                    token = get_next_token( interp );
                }
            }
            else{
                MAKE_ERR_LOC( err_loc, interp ); 
	            ERROR( "Format adresse non compatible\n");
	            return CMD_RVAL_ERROR;
            }
        }
        return CMD_RVAL_OK;
    }

    else if(0 == strcmp(token, "del")){
        token = get_next_token( interp );
        if(token==0){
			MAKE_ERR_LOC( err_loc, interp ); 
			ERROR( "Commande inconnue %s\n", err_loc );
		    return CMD_RVAL_ERROR;
		}

        else if(0 == strcmp(token, "all")){
            del_all_break(interp);
            return CMD_RVAL_OK;
        }

        else{
            while(token!=0){
                if(token_type(token)==TOK_HEXA){
                    addr1 = (uint32_t) strtol(token, &endptr, 16);
                    real_addr_mem (interp, &segm1, &indic1, addr1, &flag1);
                    if(flag1!=0){
                        MAKE_ERR_LOC( err_loc, interp ); 
			            ERROR( "L'adresse indiquée n'est pas allouée\n");
			            return CMD_RVAL_ERROR;
                    }
                    else if(0!=strcmp(interp->mips.memory->seg[segm1].name, ".text")){
                        MAKE_ERR_LOC( err_loc, interp ); 
			            ERROR( "L'adresse indiquée ne se trouve pas dans le segment .text\n");
			            return CMD_RVAL_ERROR;
                    }
                    else{
                        del_break(interp, addr1);
                        token = get_next_token( interp );
                    }
                }
                else{
                    MAKE_ERR_LOC( err_loc, interp ); 
	                ERROR( "Format adresse non compatible\n");
	                return CMD_RVAL_ERROR;
                }
            }
            return CMD_RVAL_OK;
        }

    }

    else if(0 == strcmp(token, "list")){
        print_break_list(interp);
        return CMD_RVAL_OK;
    }
}

int list_empty(break_point p){
    return NULL == p;
}

int is_break_point(interpreter interp, uint32_t addr){
    break_point b;

    for(b=interp->mips.break_p; !list_empty(b); b=b->next){
        if(b->addr_break == addr){
            return 1;
        }
    }
    return 0;
}


int del_break(interpreter interp, uint32_t addr){
    break_point b=interp->mips.break_p;
    int n;

    if(addr == 0){
        return 0;
    }
    else if(0 == is_break_point(interp, addr)){
        return 1;
    }
    else if (b->addr_break == addr){
        interp->mips.break_p = list_del_first(b);
    }
    else{
        n = list_find(b, addr);
        b = list_remove_n(n, b);
        return 1;
    }

    return 0;
}

int list_find(break_point l, uint32_t addr){
    break_point p;
    int i=1;
    for(p=l; !list_empty(p); p=p->next){
        if(p->addr_break == addr){
            return i;
        }
        i++;
    }

    return 0;
}

break_point list_remove_n(int n, break_point l){
    int i=1;
    break_point p=l;
    break_point f;

    if(n==1){
        l=list_del_first(l);
        return l;
    }

    if(n<1) return l;

    while(i<n-1 && !list_empty(p->next)){
        i++;
        p=p->next;
    }

    if(n>i+1) return l;
    f=p->next;
    p->next = f->next;
    free(f);
    return l;
}


int del_all_break(interpreter interp){
    break_point b = interp->mips.break_p;

    if(list_empty(b)){
        return 1;
    }

    while(!list_empty(b)){
        b = list_del_first(b);
    }
    interp->mips.break_p = NULL;
    return 1;
}


int add_break(interpreter interp, uint32_t addr){
    break_point b;

    if(addr == 0){
        return 0;
    }
    else if(is_break_point(interp, addr)){
        return 1;
    }
    else{
        b = calloc(1, sizeof(*b));
        if(NULL == b){
            return 1;
        }
        b->addr_break = addr;
        b->next = interp->mips.break_p;
        interp->mips.break_p = b;
        return 1;
    }

    return 0;
}


void print_break_list(interpreter interp){
    int i=0;
    break_point b = interp->mips.break_p;

    if(list_empty(b)){
        printf("Aucun breakpoint présent\n");
        return;
    }

    while(!list_empty(b)){
        printf("#%d 0x%08x\n", i, b->addr_break);
        b = b->next;
        i++;
    }
    printf("\n");
    return;
}


break_point list_del_first(break_point l){
    if(list_empty(l)){
        return NULL;
    }

    break_point p =l->next;
    free(l);
    return p;
}
