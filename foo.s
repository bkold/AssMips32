# FILE: foo.s

# Data Section

.data

a: .word 0

b: .word 2147483647

c: .word -2147483648

s1: .asciiz ""

s2: .asciiz "A"

s3: .asciiz "I am a string\n"

# Text Section

.text

main:

la $t1, a # $t1 = addr of a

lw $t0, 0($t1) # $t0 = a

addi $t0, $t0, 1 # $t0 = a + 1

sw $t0, 0($t1) # ++a

li $v0, 4 # $v0 = PrintString service code

la $a0, s3 # $a0 = addr of string s3

syscall # PrintString("I am a string\n")

jal foo # foo()

li $v0, 10 # $v0 = Exit service code

syscall # Exit()

foo:

addi $sp, $sp, -8

sw $ra, 4($sp)

sw $fp, 0($sp)

addi $fp, $sp, 4

lw $ra, -4($fp)

lw $ra, -8($fp)

addi $sp, $sp, 8

jr $ra
