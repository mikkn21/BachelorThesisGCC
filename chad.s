.data
newline: .ascii "\n"

.text
.globl _start

_start:
	movq %rsp, %rbp	# set rbp for global scope
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
	pushq $0
	pushq $0
	pushq %rbp	# setting static link
	call main
	movq $60, %rax
	xorq %rdi, %rdi
	syscall

printNum:
	movq %rdi, %rax	# The number
	movq $0, %r9	# Counter for chars to write

.LprintNum_convertLoop:
	# Convert the number to chars
	movq $0, %rdx
	movq $10, %rcx
	idivq %rcx
	addq $48, %rdx	# '0' is 48
	pushq %rdx
	addq $1, %r9
	cmpq $0, %rax
	jne .LprintNum_convertLoop

.LprintNum_printLoop:
	# Print the number to stdout
	movq $1, %rax	# sys_write
	movq $1, %rdi	# stdout
	movq %rsp, %rsi	# buf
	movq $1, %rdx	# len
	syscall
	addq $8, %rsp	# len
	addq $-1, %r9	# len
	jne .LprintNum_printLoop
	# Print newline
	movq $1, %rax	# sys_write
	movq $1, %rdi	# stdout
	movq $newline, %rsi	# buf
	movq $1, %rdx	# len
	syscall
	ret

allocate:
	pushq %rdi
	# 1. Find the current end of the data segment.
	movq $12, %rax	# brk
	xorq %rdi, %rdi	# 0 means we retrieve the current end
	syscall
	# 2. Add the amount of memory we want to allocate.
	popq %rdi	# the argument
	pushq %rax	# current end, which is where the allocated memory will start
	addq %rax, %rdi	# compute the new end
	movq $12, %rax	# brk
	syscall
	popq %rax	# the old end, which is the address of our allocated memory
	ret

L0_factorial:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
	pushq $0	# initialize local variable to 0

L1_if_statement:
	pushq $0
	movq %rbp, %r8	# starting static linking
	movq %rbp, %r9	# save RBP
	movq %r8, %rbp
	pushq %r11	# save for Generic Register Translation
	movq 24(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8	# temporarely save result
	movq %r11, 24(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq %r9, %rbp	# restore RBP
	pushq %r12	# save for Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	movq %r8, %r12	# move result to result register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	pushq $0
	pushq %r11	# save for Generic Register Translation
	movq -48(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8
	movq %r11, -48(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq $1, %r9
	xorq %r10, %r10
	cmpq %r9, %r8
	sete %r10b
	pushq %r12	# save for Generic Register Translation
	movq -56(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -56(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	pushq %r12	# save for Generic Register Translation
	movq -56(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -56(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	jne L2_endif_statement
	movq $1, %rax
	movq %rbp, %r8
	addq $-40, %r8
	movq %r8, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
	jmp L2_endif_statement

L2_endif_statement:
	pushq $0
	movq %rbp, %r8	# starting static linking
	movq %rbp, %r9	# save RBP
	movq %r8, %rbp
	pushq %r11	# save for Generic Register Translation
	movq 24(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8	# temporarely save result
	movq %r11, 24(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq %r9, %rbp	# restore RBP
	pushq %r12	# save for Generic Register Translation
	movq -64(%rbp), %r12	# Generic Register Translation
	movq %r8, %r12	# move result to result register
	movq %r12, -64(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %rsp
	movq -64(%rbp), %rdi
	call printNum
    popq %rsp
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
	pushq $0
	movq %rbp, %r8	# starting static linking
	movq %rbp, %r9	# save RBP
	movq %r8, %rbp
	pushq %r11	# save for Generic Register Translation
	movq 24(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8	# temporarely save result
	movq %r11, 24(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq %r9, %rbp	# restore RBP
	pushq %r12	# save for Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	movq %r8, %r12	# move result to result register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	pushq $0
	movq %rbp, %r8	# starting static linking
	movq %rbp, %r9	# save RBP
	movq %r8, %rbp
	pushq %r11	# save for Generic Register Translation
	movq 24(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8	# temporarely save result
	movq %r11, 24(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq %r9, %rbp	# restore RBP
	pushq %r12	# save for Generic Register Translation
	movq -80(%rbp), %r12	# Generic Register Translation
	movq %r8, %r12	# move result to result register
	movq %r12, -80(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	pushq $0
	pushq %r11	# save for Generic Register Translation
	movq -80(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8
	movq %r11, -80(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq $1, %r9
	subq %r9, %r8
	pushq %r12	# save for Generic Register Translation
	movq -88(%rbp), %r12	# Generic Register Translation
	movq %r8, %r12
	movq %r12, -88(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	pushq $0	# make space on stack
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %rsp
	pushq %r11	# save for Generic Register Translation
	movq -88(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -88(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq %rbp, %r8	# calculating static link for function call
	movq 16(%r8), %r9
	movq %r9, %r8
	pushq %r8	# setting static link
	call L0_factorial
	addq $16, %rsp	# remove arguments and static link from stack
	pushq %r12	# save for Generic Register Translation
	movq -96(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -96(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
    popq %rsp
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
	pushq $0
	pushq %r11	# save for Generic Register Translation
	movq -72(%rbp), %r11	# Generic Register Translation
	movq %r11, %r8
	movq %r11, -72(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	pushq %r11	# save for Generic Register Translation
	movq -96(%rbp), %r11	# Generic Register Translation
	movq %r11, %r9
	movq %r11, -96(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	xorq %rdx, %rdx
	imulq %r9, %r8
	pushq %r12	# save for Generic Register Translation
	movq -104(%rbp), %r12	# Generic Register Translation
	movq %r8, %r12
	movq %r12, -104(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
	pushq %r11	# save for Generic Register Translation
	movq -104(%rbp), %r11	# Generic Register Translation
	movq %r11, %rax
	movq %r11, -104(%rbp)	# move result back to Generic Register
	popq %r11	# restore after Generic Register operation
	movq %rbp, %r8
	addq $-40, %r8
	movq %r8, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

main:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
	pushq $0	# make space on stack
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %rsp
	pushq $3	# pushing int argument
	movq %rbp, %r8	# calculating static link for function call
	movq 16(%r8), %r9
	movq %r9, %r8
	pushq %r8	# setting static link
	call L0_factorial
	addq $16, %rsp	# remove arguments and static link from stack
	pushq %r12	# save for Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -48(%rbp)	# move result back to Generic Register
	popq %r12	# restore after Generic Register operation
    popq %rsp
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
	movq $0, %rax
	movq %rbp, %r8
	addq $-40, %r8
	movq %r8, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret

