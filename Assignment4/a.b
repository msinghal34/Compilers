
	.data
b:	.word 0
a:	.word 0

	.text 			# The .text assembler directive indicates
	.globl main		# The following is the code (as oppose to data)
main:				# .globl makes main know to the 
				# outside of the program.
# Prologue begins 
	sw $fp, 0($sp)		# Save the frame pointer
	sub $fp, $sp, 4		# Update the frame pointer

	sub $sp, $sp, 24		# Make space for the locals
# Prologue ends

	li $v0, 3
	sw $v0, 0($fp)
	li.d $f2, 2.40
	s.d $f2, -4($fp)
	li.d $f2, 3.00
	s.d $f2, -12($fp)
	lw $v0, 0($fp)
	li $t0, 0
	sgt $t1, $v0, $t0
	beq $t1, $zero, label0 
	li $v0, 2
	sw $v0, 0($fp)
	j label1

label0:    	
	li $v0, 1
	sw $v0, 0($fp)

label1:    	

# Epilogue Begins
	add $sp, $sp, 24
	lw $fp, 0($sp)  
	jr        $31		# Jump back to the operating system.
# Epilogue Ends

