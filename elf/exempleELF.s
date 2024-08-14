## Petit programme exemple, SANS RELOCATION
 
.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
add $t0, $t1, $t2					#ajoute t1 + t2 au registre t0
test_etiq : addi $t5, $t5, -17		#ajoute -17 au registre t5
addiu $t3, $t3, 2					#ajoute 2 au registre t3
addu $t6, $t5, $t2					#ajoute t5+t2 au registre t6
and $t7, $t1, $t2					#et logique entre les valeurs de t1 et t2 puis stocké dans t7
andi $t4, $t4, 0xffff				#et logique en 0xffff et t4 stocké dans t4
div $s0, $s1						#divise s0/s1 et stocke dans hi et lo
lui $s2, 0xffff						#charge dans le registre s2 la valeur en bit de poid fort
mfhi $s3							#charge dans s3 le contenu de hi
mflo $s4							#charge dans s4 le contenu de lo
or $s7, $s6, $s5					#ou logique entre s5 et s6 stocké dans s7
ori $t8, $s5, 0x00ff				#ou logique entre s5 et valeur stocké dans t8
seb $t9, $s6						#stocke les 8bits de poids faible de s6 en sign extent dans t9

.bss
Tab: 
.space 12 			# r�serve 12 octets en m�moire, 
					# par exemple pour stocker un tableau de 3 entiers
 
## The end
