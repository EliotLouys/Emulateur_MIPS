## Petit programme exemple, SANS RELOCATION, ne doit pas etre run
 
.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
add $t0, $t1, $t2					#ajoute t1 + t2 au registre t0
test_etiq : addi $t5, $t5, -17		#ajoute -17 au registre t5
addiu $t3, $t3, 2					#ajoute 2 au registre t3
addu $t6, $t5, $t2					#ajoute t5+t2 au registre t6
and $t7, $t1, $t2					#et logique entre les valeurs de t1 et t2 puis stocké dans t7
andi $t4, $t4, 0xffff				#et logique en 0xffff et t4 stocké dans t4
beq $t0, $t1, test_etiq
bgez $t0, test_etiq
bgtz $t0, test_etiq
blez $t0, test_etiq
bltz $t0, test_etiq
bne $t0, $t1, test_etiq
break
div $s0, $s1						#divise s0/s1 et stocke dans hi et lo
j test_etiq
jal test_etiq
jalr $t0, $t2
jr $t0
lb $t0, 0($a0)
lbu $t0, 0($a0)
lui $s2, 0xffff						#charge dans le registre s2 la valeur en bit de poid fort
lw $t0, 0($a0)
mfhi $s3							#charge dans s3 le contenu de hi
mflo $s4							#charge dans s4 le contenu de lo
nop
or $s7, $s6, $s5					#ou logique entre s5 et s6 stocké dans s7
ori $t8, $s5, 0x00ff				#ou logique entre s5 et valeur stocké dans t8
sb $t0, 0($a0)
seb $t9, $s6						#stocke les 8bits de poids faible de s6 en sign extent dans t9
sll $t0, $t1, 8                     #décalage gauche de 8 de t1 stocké dans t0
slt $t2, $t1, $t0                   #comparaison de t1 et t0 stocké dans t2
slti $t3, $t1, 0x0fff               #comparaison de t1 et valeur stocké dans t3
sltiu $t4, $t1, 0x00ff              #comparaison de t1 et valeur stocké dans t4
sltu $t5, $t1, $t0                  #comparaison de t1 et t0 stocké dans t5
sra $t6, $t7, 4                     #décalage arithmetique droite de t7 de 4 stocké dans t6
srl $t8, $t7, 4                     #décalage logique droite de t7 de 4 stocké dans t8
sub $s0, $t7, $t1                   #s0 = t7-t1 
subu $s1, $t7, $t1                  #s1 = t7-t1
sw $t0, 0($a0)
syscall                             #provoque un appel système dont les codes sont précisés dans le sujet
break                               #breakpoint
xor $s2, $s3, $s4                   #ou exclusif entre s3 et s4 stocké dns s2
mult $s5, $s6                       #s5*s6 stocké dans hi et lo
ext $t0, $t1, 5, 5



.bss
Tab: 
.space 12 			# r�serve 12 octets en m�moire, 
					# par exemple pour stocker un tableau de 3 entiers
 
## The end
