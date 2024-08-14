## Petit programme exemple, SANS RELOCATION
 
.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
sb $t3, 0($s0)
sw $t4, 0($s1)
lb $t0, 0($s0)
lbu $t1, 0($s0)
lw $t2, 0($s1)

sb $t3, 4($s2)
lb $t5, 4($s2)

sw $t4, 4($s3)
lw $t6, 4($s3)

sb $t3, -4($s2)
lb $t5, -4($s2)

sw $t4, -4($s3)
lw $t6, -4($s3)



.bss
Tab: 
.space 12 			# r�serve 12 octets en m�moire, 
					# par exemple pour stocker un tableau de 3 entiers
 
## The end
