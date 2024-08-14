/**
 * @file cmd_set.c
 * @author Eliot Louys
 * @date 16/09/2020
 * @brief Emulator command:del_register
 *
 * Emulator command: del_register
 */

#include "register.h"
#include "machine.h"

void del_register(machine vm)
{
  int i;
  for (i=0; i<35 ; i++)
  {
    vm.reg.registres_usuel[i]=0;
	free(vm.reg.num_registres[i]);
	free(vm.reg.nom_registres[i]);
  }
}

regist init_register(){
	regist reg ;
	int i;
	char *name[35] ={ "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
					"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
					"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
					"$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
					"$HI", "$LO", "$PC"};
	char *num[35] ={ "$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7",
					"$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
					"$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
					"$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31",
					"$hi", "$lo", "$pc"};

	for(i=0;i<35;i++){
		reg.nom_registres[i] = strdup(name[i]);
		reg.num_registres[i] = strdup(num[i]);
		reg.registres_usuel[i] = 0;
	}

	return reg;
}
