	add $sp, $zero, $zero, $zero, 512	# 0: set $sp = 512
	lw $a0, $zero, $zero, $zero, 256	# 1: get x from address 256
	jal $zero, $zero, $zero, $zero, fib	# 2: calc $v0 = fib(x)
	sw $v0, $zero, $zero, $zero, 257	# 3: store fib(x) in 257
	halt $zero, $zero, $zero, $zero, 0	# 4: halt


fib:	sub $sp, $sp, $zero, $zero, 3		# 5: adjust stack for 3 items
	sw $s0, $sp, $zero, $zero, 2		# 6: save $s0
	sw $ra, $sp, $zero, $zero, 1		# 7: save return address
	sw $a0, $sp, $zero, $zero, 0		# 8: save argument
        add $t1, $zero, $zero, $zero, 1		# 9: $t1 = 1
	branch $zero, $a0, $t1, 2, L1		# a: jump to L1 if x > 1
	add $v0, $a0, $zero, $zero, 0		# b: otherwise, fib(x) = x
	add $sp, $sp, $zero, $zero, 3		# c: pop 3 items from stack
	jr $ra, $zero, $zero, $zero, 0		# d: and return
L1:
	sub $a0, $a0, $zero, $zero, 1		# e: calculate x - 1
	jal $zero, $zero, $zero, $zero, fib	# f: calc $v0=fib(x-1)
	add $s0, $v0, $zero, $zero, 0		# 10: $s0 = fib(x-1)
	lw $a0, $sp, $zero, $zero, 0		# 11: restore $a0 = x
	sub $a0, $a0, $zero, $zero, 2		# 12: calculate x - 2
	jal $zero, $zero, $zero, $zero, fib	# 13: calc fib(x-2)
	add $v0, $v0, $s0, $zero, 0		# 14: $v0 = fib(x-2) + fib(x-1)
	lw $a0, $sp, $zero, $zero, 0		# 15: restore $a0
	lw $ra, $sp, $zero, $zero, 1		# 16: restore $ra
	lw $s0, $sp, $zero, $zero, 2		# 17: restore $s0
	add $sp, $sp, $zero, $zero, 3		# 18: pop 3 items from stack
	jr $ra, $zero, $zero, $zero, 0		# 19: and return
	.word 256 7
