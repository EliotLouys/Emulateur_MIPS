## Petit programme exemple, SANS RELOCATION
 
.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
slt $t0, $t1, $t2
slti $t3, $t1, 0xffff
sltiu $t4, $t1, 0xffff
sltu $t5, $t1, $t2


.bss
Tab: 
.space 12 			# r�serve 12 octets en m�moire, 
					# par exemple pour stocker un tableau de 3 entiers
 
## The end
