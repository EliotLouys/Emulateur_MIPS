

.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
ext $t0, $t1, $t2, $t3


.bss
Tab:
.space 12 			# r�serve 12 octets en m�moire,
					# par exemple pour stocker un tableau de 3 entiers

## The end
