.data
true: .ascii "true\n"
false: .ascii "false\n"
beta: .ascii "beta\n"
object: .ascii "object\n"
array: .ascii "array\n"
is_beta: .ascii "Attempted to access a beta value on line \n"
newline: .ascii "\n"

.text
.globl _start

_start:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for global scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq %rbp	# setting static link
	call main
	popq %rbp	# remove static link from stack
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	popq %rbp	# restore old rbp
	movq $60, %rax
	xorq %rdi, %rdi
	syscall

L0_fibonacci:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	movq 24(%rbp), %rbx	# move argument from stack

L1_if_statement:
	movq %rbx, %r13
	movq $1, %r12
	xorq %r10, %r10	# start of less than or equal compare
	cmpq %r12, %r13
	setle %r10b
	cmpq $1, %r10
	jne L3_else_statement
	movq %rbx, %rax
	movq %rbp, %rsp
	addq $-40, %rsp
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	movq %rbp, %rsp
	popq %rbp
	ret
	jmp L2_endif_statement

L3_else_statement:
	movq %rbx, %r12
	subq $1, %r12
	pushq %rax	# save rax
	pushq %rcx	# save rcx
	pushq %rdx	# save rdx
	pushq %rsi	# save rsi
	pushq %rdi	# save rdi
	pushq %r8	# save r8
	pushq %r9	# save r9
	pushq %r10	# save r10
	pushq %r11	# save r11
	pushq %rsp	# save rsp
	pushq %r12	# pushing register argument
	movq %rbp, %rax	# calculating static link for function call
	movq 16(%rax), %rax
	pushq %rax	# setting static link
	call L0_fibonacci
	addq $16, %rsp	# remove arguments and static link from stack
	movq %rax, %r12	# save result from function call
	popq %rsp	# restore rsp
	popq %r11	# restore r11
	popq %r10	# restore r10
	popq %r9	# restore r9
	popq %r8	# restore r8
	popq %rdi	# restore rdi
	popq %rsi	# restore rsi
	popq %rdx	# restore rdx
	popq %rcx	# restore rcx
	popq %rax	# restore rax
	subq $2, %rbx
	pushq %rax	# save rax
	pushq %rcx	# save rcx
	pushq %rdx	# save rdx
	pushq %rsi	# save rsi
	pushq %rdi	# save rdi
	pushq %r8	# save r8
	pushq %r9	# save r9
	pushq %r10	# save r10
	pushq %r11	# save r11
	pushq %rsp	# save rsp
	pushq %rbx	# pushing register argument
	movq %rbp, %rax	# calculating static link for function call
	movq 16(%rax), %rax
	pushq %rax	# setting static link
	call L0_fibonacci
	addq $16, %rsp	# remove arguments and static link from stack
	movq %rax, %rbx	# save result from function call
	popq %rsp	# restore rsp
	popq %r11	# restore r11
	popq %r10	# restore r10
	popq %r9	# restore r9
	popq %r8	# restore r8
	popq %rdi	# restore rdi
	popq %rsi	# restore rsi
	popq %rdx	# restore rdx
	popq %rcx	# restore rcx
	popq %rax	# restore rax
	movq %r12, %rax
	addq %rbx, %rax
	movq %rbp, %rsp
	addq $-40, %rsp
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	movq %rbp, %rsp
	popq %rbp
	ret

L2_endif_statement:

main:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq %rax	# save rax
	pushq %rcx	# save rcx
	pushq %rdx	# save rdx
	pushq %rsi	# save rsi
	pushq %rdi	# save rdi
	pushq %r8	# save r8
	pushq %r9	# save r9
	pushq %r10	# save r10
	pushq %r11	# save r11
	pushq %rsp	# save rsp
	pushq $46	# pushing register argument
	movq %rbp, %rax	# calculating static link for function call
	movq 16(%rax), %rax
	pushq %rax	# setting static link
	call L0_fibonacci
	addq $16, %rsp	# remove arguments and static link from stack
	popq %rsp	# restore rsp
	popq %r11	# restore r11
	popq %r10	# restore r10
	popq %r9	# restore r9
	popq %r8	# restore r8
	popq %rdi	# restore rdi
	popq %rsi	# restore rsi
	popq %rdx	# restore rdx
	popq %rcx	# restore rcx
	popq %rax	# restore rax
	xorq %rax, %rax
	movq %rbp, %rsp
	addq $-40, %rsp
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	movq %rbp, %rsp
	popq %rbp
	ret

printNum:
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	movq %rdi, %rax	# The number
	xorq %r9, %r9

.LprintNum_convertLoop:
	# Convert the number to chars
	xorq %rdx, %rdx
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
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

allocate:
	pushq %rbp
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq %rax	# save rax
	pushq %rcx	# save rcx
	pushq %rdx	# save rdx
	pushq %rsi	# save rsi
	pushq %rdi	# save rdi
	pushq %r8	# save r8
	pushq %r9	# save r9
	pushq %r10	# save r10
	pushq %r11	# save r11
	pushq %rsp	# save rsp
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
	popq %r12	# the old end, which is the address of our allocated memory
	popq %rsp	# restore rsp
	popq %r11	# restore r11
	popq %r10	# restore r10
	popq %r9	# restore r9
	popq %r8	# restore r8
	popq %rdi	# restore rdi
	popq %rsi	# restore rsi
	popq %rdx	# restore rdx
	popq %rcx	# restore rcx
	popq %rax	# restore rax
	movq %r12, %rax
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	popq %rbp
	ret

print_bool:
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	cmpq $0, %rdi
	je .print_bool_false
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $true, %rsi	# Address of 'true'
	movq $5, %rdx	# Length of string to print
	syscall
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

.print_bool_false:
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $false, %rsi	# Address of 'false'
	movq $6, %rdx	# Length of string to print
	syscall
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

print_object:
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	cmpq $0, %rdi
	je .print_object_null
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $object, %rsi	# Address of 'object'
	movq $6, %rdx	# Length of string to print
	syscall
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

.print_object_null:
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $beta, %rsi	# Address of 'beta'
	movq $4, %rdx	# Length of string to print
	syscall
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

print_array:
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	cmpq $0, %rdi
	je .print_array_null
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $array, %rsi	# Address of 'array'
	movq $5, %rdx	# Length of string to print
	syscall
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

.print_array_null:
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $beta, %rsi	# Address of 'beta'
	movq $4, %rdx	# Length of string to print
	syscall
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

print_beta:
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $beta, %rsi	# Address of 'beta'
	movq $4, %rdx	# Length of string to print
	syscall
	ret

print_is_beta:
	pushq %rdi	# Push line number
	movq $1, %rax	# System call number for write
	movq $1, %rdi	# File descriptor for stdout
	movq $is_beta, %rsi	# Address of string
	movq $41, %rdx	# Length of string to print
	syscall
	popq %rdi	# Pop line number
	call printNum
	movq $60, %rax
	movq $1, %rdi
	syscall

