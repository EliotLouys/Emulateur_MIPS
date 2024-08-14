## Petit programme exemple, SANS RELOCATION
 
.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
sll $t0, $t1, 8             #décalage gauche de 8 de t1 stocké dans t0
slt $t2, $t1, $t0           #comparaison de t1 et t0 stocké dans t2
slti $t3, $t1, 0x0fff       #comparaison de t1 et valeur stocké dans t3
sltiu $t4, $t1, 0x00ff      #comparaison de t1 et valeur stocké dans t4
sltu $t5, $t1, $t0          #comparaison de t1 et t0 stocké dans t5
sra $t6, $t7, 4             #décalage arithmetique droite de t7 de 4 stocké dans t6
srl $t8, $t7, 4             #décalage logique droite de t7 de 4 stocké dans t8
sub $s0, $t7, $t1           #s0 = t7-t1 
subu $s1, $t7, $t1          #s1 = t7-t1
syscall                     #provoque un appel système dont les codes sont précisés dans le sujet
xor $s2, $s3, $s4           #ou exclusif entre s3 et s4 stocké dns s2
mult $s5, $s6               #s5*s6 stocké dans hi et lo
break                       #breakpoint



.bss
Tab: 
.space 12 			# r�serve 12 octets en m�moire, 
					# par exemple pour stocker un tableau de 3 entiers
 
## The end
