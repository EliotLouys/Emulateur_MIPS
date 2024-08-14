
/**
 * @file machine.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Mon Aug 24 09:35:17 2020
 * @brief Virtual machine.
 *
 * Virtual machine.
 */

#include "machine.h"


/**
 * @brief Initialize virtual machine information.
 * @param vm Pointer to virtual machine.
 */
void init_machine( machine *vm ) {
	int i;
  vm->symtab = new_stab( 0 );
  vm->reg = init_register();
  vm->break_p = NULL;
  
  for(i=0;i<32;i++){
	  DEBUG("le nom du registre est %s et son numéro est %s\n", vm->reg.nom_registres[i], vm->reg.num_registres[i]);
  }
  
}

/**
 * @brief Delete virtual machine informations.
 * @param vm Virtual machine.
 */
void del_machine( machine vm ) {
  del_mem( vm.memory );
  del_stab( vm.symtab );
  del_register(vm);
}
