## Petit programme exemple, SANS RELOCATION
 
.data
X : .word 0xabc 	# Attention, relocation des que X est reference...

.text
addi $t0, $t0, -4
nop
bne $t2, $t1, step1

step1: addi $t2, $t2, -5
nop
beq $t2, $t0, step2
nop

step2: addi $t3, $t3, 40
nop
bgez $t4, step3
nop

step3: addi $t4, $t4, 3
nop
bgtz $t3, step4
nop

step4: sub $t5, $t5, 0
nop
blez $t5, step5
nop

step5: sub $t5, $t5, 15
bltz $t5, step6
nop

step6: addi $t5, $t5, 10



.bss
Tab: 
.space 12 			# r�serve 12 octets en m�moire, 
					# par exemple pour stocker un tableau de 3 entiers
 
## The end
