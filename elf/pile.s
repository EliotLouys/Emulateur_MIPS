# programme qui fait appel a une procedure (JAL) et utilise la pile
# se contente de verifier qu'un decalage Ã  gauche puis Ã  droite donne la valeur initiale

#!!! Attention il faut charger le text de la section elf Ã  0x5000 (pas de reloc) 

.set noreorder    # ne pas modifier l'ordre des instructions ne fait meme pas des optimisations de boucle (les NOP sont mis de maniÃ¨re explicite)

_start:
.global _start

addi $t1,$zero,8
add $t2,$zero,$t1
SLL $t1,$t1,2
JAL 0x5024      # appel explicite Ã  l'adresse de la procedure  
NOP
BEQ $t1,$t2,OK  # branchement Ã  OK si les deux valeurs sont Ã©gales
NOP
J 0x505C 						# sinon saut Ã  KO
NOP


procedure:
 SW $31,0($sp)   # on stocke l'adresse de retour
 ADDI $sp,$sp,-4 # on decremente le pointeur de pile
 SW $t1,0($sp)   # on stocke les registres sur lesquels la procedure agit (ici $t1 seulement)
 ADDI $sp,$sp,-4

 SRL $t1,$t1,2

 ADDI $sp,$sp,4
 NOP             # on ne recupere pas $t1 car c'est la valeur de retour
 ADDI $sp,$sp,4
 LW $31,0($sp)   # on recupere l'adresse de retour

 JR $31
nop

OK:
ADDI $v0,$zero,4  # appel explicite Ã  print
ADDI $a0,$zero,0x6000  # la chaine se trouve en dÃ©but de section data 
syscall #affiche la chaine
KO:
ADDI $v0,$zero,10  # appel explicite Ã  exit
syscall

.data 
chaine: .asciiz "Youpi"