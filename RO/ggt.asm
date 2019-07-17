# Hauptprogramm
	.text
   	.globl main
main:
	addi	$sp,	$sp,	-4		
    	sw 	$ra,	0($sp)       	
	jal	setup				
	jal	func				
	add	$a0,	$zero,	$v0		
	jal 	printInt
    	lw    $ra,  	0($sp)       	
    	addi  $sp,  	$sp, 	4       
    	jr    $ra

printInt:
    	li    $v0, 	1  
    	syscall
    	jr    $ra   

readInt:
    	li    $v0, 	5 
    	syscall
    	jr    $ra    
    
func:
	addi 	$v0,	$zero,	0
	addi 	$t0,	$zero,	0
	addi 	$s0, 	$a1,	0
	addi	$s1, 	$a2, 	0
	
	loop:
		beq	$t0,	$a0,	end
		
		lw	$t1, 	0($s0)		
		lw	$t2, 	0($s1)		
		mult 	$t1, 	$t2		
		mflo	$t1			
		add	$v0, 	$v0, 	$t1	
		
		addi	$t0,	$t0,	1
		addi	$s0, 	$s0, 	-4	
		addi 	$s1, 	$s1, 	-4	
		j	loop
	end:
		jr 	$ra
    
setup:
	addi 	$a0,	$zero,	3
	
	addi 	$t0,	$zero,	3
	sub 	$sp,	$sp,	4
	sw	$t0,	($sp)
	add	$a1,	$zero,	$sp
	
	addi 	$t0,	$zero,	1
	sub 	$sp,	$sp,	4
	sw	$t0,	($sp)
	
	addi 	$t0,	$zero,	-3
	sub 	$sp,	$sp,	4
	sw	$t0,	($sp)			
	
	addi 	$t0,	$zero,	1
	sub 	$sp,	$sp,	4
	sw	$t0,	($sp)
	add	$a2,	$zero,	$sp	
	
	addi 	$t0,	$zero,	1
	sub 	$sp,	$sp,	4
	sw	$t0,	($sp)	
	
	addi 	$t0,	$zero,	2
	sub 	$sp,	$sp,	4
	sw	$t0,	($sp)	
	
	jr 	$ra