
#include "ins_ass.h"
#include "commands.h"

//Instructions vérifiées

int exec_add(interpreter interp)
{
  int64_t temp;
  int32_t rs_temp = interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  int32_t rt_temp = interp->mips.reg.registres_usuel[interp->word_value.asR.rt];

  temp = (long) rs_temp + (long) rt_temp;
  if (temp > 4294967295 || temp < -2147483648)
  {
    DEBUG("Overflow exception \n");
    return 0;
  }
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = (uint32_t) temp;
  return 1;
}

int exec_addi(interpreter interp)
{
  int32_t immediate_extend;
  int64_t temp;
  int32_t rs_temp = interp->mips.reg.registres_usuel[interp->word_value.asI.rs];

  if (interp->word_value.asI.immediate & (1 << 15)){
    DEBUG("Signe négatif détecté\n");
    immediate_extend =  0xffff0000 | interp->word_value.asI.immediate;
  }
  else if (!(interp->word_value.asI.immediate & (1 << 15))){
    DEBUG("Signe positif détecté\n");
    immediate_extend =  0x00000000 | interp->word_value.asI.immediate;
  }

  temp = (long) rs_temp + (long) immediate_extend;
  DEBUG("temp = %lx\n", temp);
  DEBUG("rs_temp = %x\n", rs_temp);
  DEBUG("immediate_extend = %x\n", immediate_extend);

  if (temp>4294967295 || temp<-2147483648)
  {
    DEBUG("Erreur d'overflow\n");
    return 0;
  }

  interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = (uint32_t) temp;
  return 1;
}

int exec_addiu(interpreter interp)
{
  uint32_t immediate_extend, temp;

  if (interp->word_value.asI.immediate & (1 << 15)){
    immediate_extend =  0xffff0000 | interp->word_value.asI.immediate;
  }
  else if (!(interp->word_value.asI.immediate & (1 << 15))){
    immediate_extend =  0x00000000 | interp->word_value.asI.immediate;
  }

  temp = interp->mips.reg.registres_usuel[interp->word_value.asI.rs] + immediate_extend;
  interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = temp;
  return 1;
}

int exec_addu(interpreter interp)
{
  DEBUG("Registre rd:%s\nregistre rs:%s\nregistre rt:%s\n",interp->mips.reg.nom_registres[interp->word_value.asR.rd], interp->mips.reg.nom_registres[interp->word_value.asR.rs] ,interp->mips.reg.nom_registres[interp->word_value.asR.rt]);
  DEBUG("valeur registre rd:%d\nvaleur registre rs:%d\nvaleur registre rt:%d\n",interp->mips.reg.registres_usuel[interp->word_value.asR.rd], interp->mips.reg.registres_usuel[interp->word_value.asR.rs], interp->mips.reg.registres_usuel[interp->word_value.asR.rt] );
  DEBUG("Valeur de l'addition: %d\n", interp->mips.reg.registres_usuel[interp->word_value.asR.rs] + interp->mips.reg.registres_usuel[interp->word_value.asR.rt]);
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd]=interp->mips.reg.registres_usuel[interp->word_value.asR.rs] + interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  return 1;
}

int exec_and(interpreter interp)
{
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd]=interp->mips.reg.registres_usuel[interp->word_value.asR.rs] & interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  return 1;
}

int exec_andi(interpreter interp)
{
  uint32_t immediate_extend;

  immediate_extend =  0x00000000 | interp->word_value.asI.immediate;
  interp->mips.reg.registres_usuel[interp->word_value.asI.rt]=interp->mips.reg.registres_usuel[interp->word_value.asI.rs] & immediate_extend;
  return 1;
}

int exec_div(interpreter interp)
{
  DEBUG("Rentrée dans la focntion div\n");
  DEBUG("Registre rs:%s\nRegistre rt:%s\n",interp->mips.reg.nom_registres[interp->word_value.asR.rs],interp->mips.reg.nom_registres[interp->word_value.asR.rt]);
  if (interp->mips.reg.registres_usuel[interp->word_value.asR.rt] == 0){
    DEBUG("Division par 0 on ne fait rien\n");
    return 3;
  }
  interp->mips.reg.registres_usuel[33]=interp->mips.reg.registres_usuel[interp->word_value.asR.rs] / interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  interp->mips.reg.registres_usuel[32]=interp->mips.reg.registres_usuel[interp->word_value.asR.rs] % interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  return 1;
}

int exec_lui(interpreter interp){
  uint32_t res, temp;
  DEBUG("Rentrée dans  la fonction LUI\n");
  DEBUG("le registre rt est %s:\n", interp->mips.reg.nom_registres[interp->word_value.asI.rt]);
  temp = interp->word_value.asI.immediate<<16;
  res = temp | 0x0000;
  DEBUG("La valeur que l'on va stocker dans le registre est:0x%x\n", res);
  interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = res;
  return 1;
}

int exec_mfhi(interpreter interp)
{
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd]=interp->mips.reg.registres_usuel[32];
  return 1;
}

int exec_mflo(interpreter interp)
{
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd]=interp->mips.reg.registres_usuel[33];
  return 1;
}

int exec_mult(interpreter interp){
  long long prod;
  int32_t rs_temp, rt_temp;

  rs_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  rt_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  prod = rs_temp * rt_temp ;
  DEBUG("Le produit de %x * %x vaut: 0x%llx\n",rs_temp, rt_temp, prod);
  interp->mips.reg.registres_usuel[33] = prod & 0x00000000ffffffff; //LO
  interp->mips.reg.registres_usuel[32] = (prod & 0xffffffff00000000)>>32; //HI
  return 1;
}

int exec_nop()
{
  return 1;
}

int exec_or(interpreter interp)
{
  DEBUG("Registre rd:%s\nregistre rs:%s\nregistre rt:%s\n",interp->mips.reg.nom_registres[interp->word_value.asR.rd], interp->mips.reg.nom_registres[interp->word_value.asR.rs] ,interp->mips.reg.nom_registres[interp->word_value.asR.rt]);
  DEBUG("valeur registre rd:%d\nvaleur registre rs:%d\nvaleur registre rt:%d\n",interp->mips.reg.registres_usuel[interp->word_value.asR.rd], interp->mips.reg.registres_usuel[interp->word_value.asR.rs], interp->mips.reg.registres_usuel[interp->word_value.asR.rt] );
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = interp->mips.reg.registres_usuel[interp->word_value.asR.rt] | interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  return 1;
}

int exec_ori(interpreter interp)
{
  uint32_t zero_extent;
  DEBUG("Registre rs:%s\nRegistre rt:%s\n",interp->mips.reg.nom_registres[interp->word_value.asI.rs],interp->mips.reg.nom_registres[interp->word_value.asI.rt]);
  DEBUG("Valeur de immediate: %d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur registre rs:%d\nValeur registre rt:%d\n",interp->mips.reg.registres_usuel[interp->word_value.asI.rs],interp->mips.reg.registres_usuel[interp->word_value.asI.rt]);
  zero_extent = (0x0000<<16) | interp->word_value.asI.immediate;
  DEBUG("zero extent = 0x%x\n", zero_extent);
  interp->mips.reg.registres_usuel[interp->word_value.asI.rt]=interp->mips.reg.registres_usuel[interp->word_value.asI.rs] | zero_extent;
  return 1;
}

int exec_seb(interpreter interp){
  uint8_t sign_detect;
  uint32_t res;

  sign_detect = interp->mips.reg.registres_usuel[interp->word_value.asR.rt] & 0x000000ff;
  DEBUG("sign detect: 0x%x\n", sign_detect);
  if ( sign_detect & (1 << 7) ) {
    res = 0xffffff00 | sign_detect;
    DEBUG("Entrée dans bit signe 1\n");
    DEBUG("res: 0x%x\n", res);
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = res;
    return 1;
  }
  else if ( !(sign_detect & (1 << 7)) ) {
    res = 0x00000000 | sign_detect;
    DEBUG("Entrée dans bit signe 0\n");
    DEBUG("res: 0x%x\n", res);
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = res;
    return 1;
  }

  return 404;
}

int exec_sll(interpreter interp){
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = interp->mips.reg.registres_usuel[interp->word_value.asR.rt] << interp->word_value.asR.sa;
  return 1;
}

int exec_slt(interpreter interp){
  int32_t rs_cmp, rt_cmp;

  rs_cmp = (signed) interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  rt_cmp = (signed) interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  
  if(rs_cmp<rt_cmp){
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = 1;
  }
  else{
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = 0;
  }
  return 1;
}

int exec_slti(interpreter interp)
{
  int32_t rs_cmp, immediate_cmp_extend;

  rs_cmp = (signed) interp->mips.reg.registres_usuel[interp->word_value.asI.rs];

  if (interp->word_value.asI.immediate & (1 << 15)){
    immediate_cmp_extend =  0xffff0000 | interp->word_value.asI.immediate;
  }
  else if (!(interp->word_value.asI.immediate & (1 << 15))){
    immediate_cmp_extend =  0x00000000 | interp->word_value.asI.immediate;
  }

  if (rs_cmp < immediate_cmp_extend)
  {
    interp->mips.reg.registres_usuel[interp->word_value.asI.rt]=1;
  }
  else
  {
    interp->mips.reg.registres_usuel[interp->word_value.asI.rt]=0;
  }
  return 1;
}

int exec_sltiu(interpreter interp){
  uint32_t immediate_cmp_extend;

  if (interp->word_value.asI.immediate & (1 << 15)){
    immediate_cmp_extend =  0xffff0000 | interp->word_value.asI.immediate;
  }
  else if (!(interp->word_value.asI.immediate & (1 << 15))){
    immediate_cmp_extend =  0x00000000 | interp->word_value.asI.immediate;
  }

  if (interp->mips.reg.registres_usuel[interp->word_value.asI.rs] < immediate_cmp_extend)
  {
    interp->mips.reg.registres_usuel[interp->word_value.asI.rt]=1;
  }
  else
  {
    interp->mips.reg.registres_usuel[interp->word_value.asI.rt]=0;
  }
  return 1;
}

int exec_sltu(interpreter interp){
  if(interp->mips.reg.registres_usuel[interp->word_value.asR.rs]<interp->mips.reg.registres_usuel[interp->word_value.asR.rt]){
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = 1;
  }
  else{
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = 0;
  }
  return 1;
}

int exec_sra(interpreter interp){
  int32_t rt_temp;

  rt_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = rt_temp >> interp->word_value.asR.sa;
  return 1;
}

int exec_srl(interpreter interp){
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = interp->mips.reg.registres_usuel[interp->word_value.asR.rt] >> interp->word_value.asR.sa;
  return 1;
}

int exec_sub(interpreter interp){
  long long temp;
  long rs_temp = interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  long rt_temp = interp->mips.reg.registres_usuel[interp->word_value.asR.rt];

  temp = rs_temp - rt_temp;
  DEBUG("temp = %lld\n", temp);
  DEBUG("rs_temp = %ld\n", rs_temp);
  DEBUG("rt_temp = %ld\n", rt_temp);
  if(temp>4294967295 || temp<-2147483648){
    DEBUG("Erreur d'overflow\n");
    return 0;
  }
  else{
    interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = (uint32_t) temp;
    return 1;
  }
}

int exec_subu(interpreter interp){
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = interp->mips.reg.registres_usuel[interp->word_value.asR.rs] - interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  return 1;
}

int exec_syscall(interpreter interp){
  int code = interp->mips.reg.registres_usuel[2];
  int segm1, indic1, flag1, lecture_val,j;
  //int taille_tampon;
  //char* chaine_lue;

  switch (code){
  case 1:
    printf("%d\n",interp->mips.reg.registres_usuel[4]);
    return 1;
  break;

  case 4:
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[4], &flag1);
    if(flag1!=0){
			return 404;
		}
    DEBUG("adresse de la chaine: 0x%x\n", interp->mips.memory->seg[segm1].content[indic1]);
    
    j=0;
    while(interp->mips.memory->seg[segm1].content[indic1+j] != '\0'){
      printf("%c", interp->mips.memory->seg[segm1].content[indic1+j]);
      j++;
    }
    printf("\n");
    return 1;
  break;

  case 5:
    scanf("%d", &lecture_val);
    interp->mips.reg.registres_usuel[2] = lecture_val;
  return 1;
  break;

  case 8:
    /*gets("%s", chaine_lue);
    taille_tampon = strlen(chaine_lue);
    interp->mips.reg.registres_usuel[4] = ;//reg a0
    interp->mips.reg.registres_usuel[5] = chaine_lue;//reg a5*/
    return 10; //non traité pour l'instant --> fin du programme
  break;

  case 10:
    return 10;//fin du programme, rend la main a l'utilisateur
  break;
  
  default:
    return 404;//fin du programme suite a une erreur, rend la main a l'utilisateur
  break;
  }
}

int exec_xor(interpreter interp){
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = interp->mips.reg.registres_usuel[interp->word_value.asR.rs] ^ interp->mips.reg.registres_usuel[interp->word_value.asR.rt];
  return 1;
}

int exec_break()
{
  return 5;
}

int exec_bne(interpreter interp, instruct_def *inst)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_saut;
  int segm1, indic1, flag1;

  DEBUG("Valeur du saut avant cast:%d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur du saut après cast:%d\n", off_temp);
  
  if (interp->mips.reg.registres_usuel[interp->word_value.asI.rs] != interp->mips.reg.registres_usuel[interp->word_value.asI.rt])
  {
    addr_saut = interp->mips.reg.registres_usuel[34] + off_temp * 4;
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
    if(flag1!=0 ){
		  ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		  return 404;
	  }
    if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
      printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
      return 2;
    }
    else{
      disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
      if(interp->dico == -1){
		    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		    return 404;
      }
      execute_inst(interp, inst);
    }
    interp->mips.reg.registres_usuel[34] = addr_saut;
  }
return 1;
}

int exec_beq(interpreter interp, instruct_def *inst)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_saut;
  int segm1, indic1, flag1;

  DEBUG("Valeur du saut avant cast:%d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur du saut après cast:%d\n", off_temp);
  
  if (interp->mips.reg.registres_usuel[interp->word_value.asI.rs] == interp->mips.reg.registres_usuel[interp->word_value.asI.rt])
  {
    addr_saut = interp->mips.reg.registres_usuel[34] + off_temp * 4;
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
    if(flag1!=0 ){
		  ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		  return 404;
	  }
    if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
      printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
      return 2;
    }
    else{
      disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
      if(interp->dico == -1){
		    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		    return 404;
      }
      execute_inst(interp, inst);
    }
    interp->mips.reg.registres_usuel[34] = addr_saut;
  }
return 1;
}

int exec_bgez(interpreter interp, instruct_def *inst)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  int32_t rs_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asI.rs];
  uint32_t addr_saut;
  int segm1, indic1, flag1;

  DEBUG("Valeur du saut avant cast:%d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur du saut après cast:%d\n", off_temp);
  
  if (rs_temp>=0)
  {
    addr_saut = interp->mips.reg.registres_usuel[34] + off_temp * 4;
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
    if(flag1!=0 ){
		  ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		  return 404;
	  }
    if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
      printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
      return 2;
    }
    else{
      disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
      if(interp->dico == -1){
		    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		    return 404;
      }
      execute_inst(interp, inst);
    }
    interp->mips.reg.registres_usuel[34] = addr_saut;
  }
return 1;
}

int exec_bgtz(interpreter interp, instruct_def *inst)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  int32_t rs_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asI.rs];
  uint32_t addr_saut;
  int segm1, indic1, flag1;

  DEBUG("Valeur du saut avant cast:%d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur du saut après cast:%d\n", off_temp);
  
  if (rs_temp>0)
  {
    addr_saut = interp->mips.reg.registres_usuel[34] + off_temp * 4;
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
    if(flag1!=0 ){
		  ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		  return 404;
	  }
    if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
      printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
      return 2;
    }
    else{
      disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
      if(interp->dico == -1){
		    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		    return 404;
      }
      execute_inst(interp, inst);
    }
    interp->mips.reg.registres_usuel[34] = addr_saut;
  }
return 1;
}

int exec_blez(interpreter interp, instruct_def *inst)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  int32_t rs_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asI.rs];
  uint32_t addr_saut;
  int segm1, indic1, flag1;

  DEBUG("Valeur du saut avant cast:%d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur du saut après cast:%d\n", off_temp);
  
  if (rs_temp<=0)
  {
    addr_saut = interp->mips.reg.registres_usuel[34] + off_temp * 4;
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
    if(flag1!=0 ){
		  ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		  return 404;
	  }
    if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
      printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
      return 2;
    }
    else{
      disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
      if(interp->dico == -1){
		    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		    return 404;
      }
      execute_inst(interp, inst);
    }
    interp->mips.reg.registres_usuel[34] = addr_saut;
  }
return 1;
}

int exec_bltz(interpreter interp, instruct_def *inst)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  int32_t rs_temp = (int32_t) interp->mips.reg.registres_usuel[interp->word_value.asI.rs];
  uint32_t addr_saut;
  int segm1, indic1, flag1;

  DEBUG("Valeur du saut avant cast:%d\n", interp->word_value.asI.immediate);
  DEBUG("Valeur du saut après cast:%d\n", off_temp);
  
  if (rs_temp<0)
  {
    addr_saut = interp->mips.reg.registres_usuel[34] + off_temp * 4;
    real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
    if(flag1!=0 ){
		  ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
		  return 404;
	  }
    if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
      printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
      return 2;
    }
    else{
      disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
      if(interp->dico == -1){
		    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		    return 404;
      }
      execute_inst(interp, inst);
    }
    interp->mips.reg.registres_usuel[34] = addr_saut;
  }
return 1;
}

int exec_j(interpreter interp, instruct_def *inst)
{
  uint32_t addr_saut;
  int segm1, indic1, flag1;
  
  addr_saut = (interp->mips.reg.registres_usuel[34] & 0xf0000000) + (interp->word_value.asJ.target << 2);
  real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
  if(flag1!=0 ){
	ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
	return 404;
	}
  if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
    printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
    return 2;
  }
  else{
    disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
    if(interp->dico == -1){
	    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		  return 404;
    }
    execute_inst(interp, inst);
  }
  interp->mips.reg.registres_usuel[34] = addr_saut;
return 1;
}

int exec_jal(interpreter interp, instruct_def *inst)
{
  uint32_t addr_saut;
  int segm1, indic1, flag1;
  
  interp->mips.reg.registres_usuel[31] = interp->mips.reg.registres_usuel[34] + 4;
  addr_saut = (interp->mips.reg.registres_usuel[34] & 0xf0000000) + (interp->word_value.asJ.target << 2);
  real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
  if(flag1!=0 ){
	ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
	return 404;
	}
  if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
    printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
    return 2;
  }
  else{
    disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
    if(interp->dico == -1){
	    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		  return 404;
    }
    execute_inst(interp, inst);
  }
  interp->mips.reg.registres_usuel[34] = addr_saut;
return 1;
}

int exec_jr(interpreter interp, instruct_def *inst)
{
  uint32_t addr_saut;
  int segm1, indic1, flag1;
  
  addr_saut = interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
  if(flag1!=0 ){
	ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
	return 404;
	}
  if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
    printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
    return 2;
  }
  else{
    disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
    if(interp->dico == -1){
	    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		  return 404;
    }
    execute_inst(interp, inst);
  }
  interp->mips.reg.registres_usuel[34] = addr_saut;
return 1;
}

int exec_jalr(interpreter interp, instruct_def *inst)
{
  uint32_t addr_saut;
  int segm1, indic1, flag1;
  
  addr_saut = interp->mips.reg.registres_usuel[interp->word_value.asR.rs];
  interp->mips.reg.registres_usuel[interp->word_value.asR.rd] = interp->mips.reg.registres_usuel[34] + 4;
  real_addr_mem (interp, &segm1, &indic1, interp->mips.reg.registres_usuel[34], &flag1);//Calcul de l'adresse reelle du PC
  if(flag1!=0 ){
	ERROR( "L'adresse chargée dans le PC n'existe pas ou n'est pas allouée\n");
	return 404;
	}
  if(is_break_point(interp, interp->mips.reg.registres_usuel[34])){
    printf("Présence d'un breakpoint à l'adresse PC:0x%x\n", interp->mips.reg.registres_usuel[34]);
    return 2;
  }
  else{
    disasm(interp, inst, segm1, indic1, 0);//Désassemblage a l'adresse pour trouver l'instruction à éxécuter
    if(interp->dico == -1){
	    ERROR( "Erreur d'éxécution: instruction non trouvée\n");
		  return 404;
    }
    execute_inst(interp, inst);
  }
  interp->mips.reg.registres_usuel[34] = addr_saut;
return 1;
}

int exec_lb(interpreter interp)
{
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_mem, res;
  int segm1, indic1, flag1;

  addr_mem = interp->mips.reg.registres_usuel[interp->word_value.asI.rs] + off_temp;
  real_addr_mem (interp, &segm1, &indic1, addr_mem, &flag1);

  if(flag1!=0){
		return 404;
	}

  if ( interp->mips.memory->seg[segm1].content[indic1] & (1 << 7) ) {
    res = 0xffffff00 | interp->mips.memory->seg[segm1].content[indic1];
    DEBUG("Entrée dans bit signe 1\n");
    DEBUG("res: 0x%x\n", res);
    interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = res;
    return 1;
  }
  else if ( !(interp->mips.memory->seg[segm1].content[indic1] & (1 << 7)) ) {
    res = 0x00000000 | interp->mips.memory->seg[segm1].content[indic1];
    DEBUG("Entrée dans bit signe 0\n");
    DEBUG("res: 0x%x\n", res);
    interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = res;
    return 1;
  }

  return 404;
}

int exec_lbu(interpreter interp){
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_mem, res;
  int segm1, indic1, flag1;

  addr_mem = interp->mips.reg.registres_usuel[interp->word_value.asI.rs] + off_temp;
  real_addr_mem (interp, &segm1, &indic1, addr_mem, &flag1);

  if(flag1!=0){
		return 404;
	}

  res = 0x00000000 | interp->mips.memory->seg[segm1].content[indic1];
  interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = res;
  return 1;
}

int exec_lw(interpreter interp){
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_mem, res;
  int segm1, indic1, flag1;
  uint32_t m0, m1, m2, m3;

  addr_mem = interp->mips.reg.registres_usuel[interp->word_value.asI.rs] + off_temp;
  real_addr_mem (interp, &segm1, &indic1, addr_mem, &flag1);

  if(flag1!=0){
		return 404;
	}

  
  m0 = interp->mips.memory->seg[segm1].content[indic1+3];
	m1 = interp->mips.memory->seg[segm1].content[indic1+2];
	m2 = interp->mips.memory->seg[segm1].content[indic1+1];
	m3 = interp->mips.memory->seg[segm1].content[indic1];
  res = (m3<<24) | (m2<<16) | (m1<<8) | m0;
  
  interp->mips.reg.registres_usuel[interp->word_value.asI.rt] = res;
  return 1;
}

int exec_sb(interpreter interp){
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_mem;
  byte res;
  int segm1, indic1, flag1;

  addr_mem = interp->mips.reg.registres_usuel[interp->word_value.asI.rs] + off_temp;
  real_addr_mem (interp, &segm1, &indic1, addr_mem, &flag1);

  if(flag1!=0){
		return 404;
	}

  res = 0xff & interp->mips.reg.registres_usuel[interp->word_value.asI.rt];
  DEBUG("valeur de res:0x%x\n", res);

  interp->mips.memory->seg[segm1].content[indic1] = res;
  return 1;
}

int exec_sw(interpreter interp){
  int16_t off_temp = (int16_t) interp->word_value.asI.immediate;
  uint32_t addr_mem;
  byte m0, m1, m2, m3;
  int segm1, indic1, flag1;

  addr_mem = interp->mips.reg.registres_usuel[interp->word_value.asI.rs] + off_temp;
  real_addr_mem (interp, &segm1, &indic1, addr_mem, &flag1);

  if(flag1!=0){
    DEBUG("adresse non trouvée\n");
		return 404;
	}

  m0 = interp->mips.reg.registres_usuel[interp->word_value.asI.rt] & 0xFF;
	m1 = (interp->mips.reg.registres_usuel[interp->word_value.asI.rt] & 0xFF00)>>8;
	m2 = (interp->mips.reg.registres_usuel[interp->word_value.asI.rt] & 0xFF0000)>>16;
	m3 = (interp->mips.reg.registres_usuel[interp->word_value.asI.rt] & 0xFF000000)>>24;

  interp->mips.memory->seg[segm1].content[indic1+3] = m0;
  interp->mips.memory->seg[segm1].content[indic1+2] = m1;
  interp->mips.memory->seg[segm1].content[indic1+1] = m2;
  interp->mips.memory->seg[segm1].content[indic1] = m3;

  return 1;
}

int exec_ext(interpreter interp){

  uint8_t msbd = interp->word_value.asR.rd-1;
  uint8_t lsb = interp->word_value.asR.sa;
  uint32_t temp, mask1, mask2, mask;

  if( ( msbd+lsb ) > 31){
    return 404;
  }

  //création du mask
  mask1 = 0xffffffff<<lsb;
  mask2 = 0xffffffff >> msbd;
  mask = mask1 & mask2;
  temp = interp->mips.reg.registres_usuel[interp->word_value.asR.rs] & mask;

  interp->mips.reg.registres_usuel[interp->word_value.asR.rt] =  0x00000000 | temp; //zero_extent
  
  return 1;
}





