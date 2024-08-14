
/**
 * @file machine.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sun Aug 23 11:15:04 2020
 * @brief MIPS32 virtual machine.
 *
 * MIPS32 virtual machine.
 */

#include "mem.h"
#include "register.h"
#include "elf/syms.h"


#ifndef _MACHINE_H_
#define _MACHINE_H_

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief Virtual machine description.
   * @todo Add registers!
   */
   
  typedef struct {
    char *nom_registres[35];					
	  char *num_registres[35];					
	  uint32_t registres_usuel[35];
} regist;

  typedef struct {

    mem         memory;  /**< Virtual memory. */
    stab        symtab;  /**< Symbol table. */
	  regist 	    reg;      /*registre de la machine*/
    break_point break_p;
  } machine;



  void init_machine( machine *vm );
  void del_machine( machine vm );
  void del_register(machine vm);
  

  
  regist init_register();
  //break_point init_break_tab();


#ifdef __cplusplus
}
#endif

#endif /* _MACHINE_H_ */
