
.globl main	

.data
    newline: .ascii "\n"

.text
	

main:
    movq $23324, %rdi
    call printNum

    #exit
    movq $60, %rax
    xorq %rdi, %rdi
    syscall


printNum:
	movq %rdi, %rax # The number

	movq $0, %r9 # Counter for chars to write
	# Convert the number to chars
.LprintNum_convertLoop:
	movq $0, %rdx
	movq $10, %rcx
	idivq %rcx
	addq $48, %rdx # '0' is 48
	pushq %rdx
	addq $1, %r9
	cmpq $0, %rax   
	jne .LprintNum_convertLoop
	# Print the number to stdout
.LprintNum_printLoop:
	movq $1, %rax # sys_write
	movq $1, %rdi # stdout
	movq %rsp, %rsi # buf
	movq $1, %rdx # len
	syscall
	addq $8, %rsp
	addq $-1, %r9
	jne .LprintNum_printLoop
.print_newline:
    movq $1, %rax
    movq $1, %rdi
    movq $newline, %rsi
    movq $1, %rdx
    syscall
    ret


