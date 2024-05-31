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

L0_merge:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	movq 40(%rbp), %r11	# Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -56(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -56(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r11	# Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12
	movq %r11, -56(%rbp)	# move result back to Generic Register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	movq -48(%rbp), %r11	# Generic Register Translation
	movq -64(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq %r12, -64(%rbp)	# move result back to Generic Register
	movq -64(%rbp), %r11	# Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -64(%rbp)	# move result back to Generic Register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -80(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -80(%rbp)	# move result back to Generic Register
	movq -80(%rbp), %r11	# Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -80(%rbp)	# move result back to Generic Register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -72(%rbp), %r11	# Generic Register Translation
	movq -88(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -72(%rbp)	# move result back to Generic Register
	movq %r12, -88(%rbp)	# move result back to Generic Register
	movq -88(%rbp), %r11	# Generic Register Translation
	movq -96(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -88(%rbp)	# move result back to Generic Register
	movq %r12, -96(%rbp)	# move result back to Generic Register
	movq 48(%rbp), %r11	# Generic Register Translation
	movq -104(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 48(%rbp)	# move result back to Generic Register
	movq %r12, -104(%rbp)	# move result back to Generic Register
	movq 40(%rbp), %r11	# Generic Register Translation
	movq -112(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq %r12, -112(%rbp)	# move result back to Generic Register
	movq -112(%rbp), %r11	# Generic Register Translation
	movq -104(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12
	movq %r11, -112(%rbp)	# move result back to Generic Register
	movq %r12, -104(%rbp)	# move result back to Generic Register
	movq -104(%rbp), %r11	# Generic Register Translation
	movq -120(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -104(%rbp)	# move result back to Generic Register
	movq %r12, -120(%rbp)	# move result back to Generic Register
	movq -120(%rbp), %r11	# Generic Register Translation
	movq -128(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -120(%rbp)	# move result back to Generic Register
	movq %r12, -128(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r12	# Generic Register Translation
	movq $8, %r12	# initialize memory size
	movq %r12, -136(%rbp)	# move result back to Generic Register
	movq -96(%rbp), %r11	# Generic Register Translation
	movq -136(%rbp), %r12	# Generic Register Translation
	imulq %r11, %r12	# calculate memory
	movq %r11, -96(%rbp)	# move result back to Generic Register
	movq %r12, -136(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# found size of memory
	movq %r12, -136(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r11	# Generic Register Translation
	movq %r11, %rdi	# set memory size for allocation
	movq %r11, -136(%rbp)	# move result back to Generic Register
	call allocate	# allocate memory of found memory size
	movq -144(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# Save array pointer in generic register
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -96(%rbp), %r11	# Generic Register Translation
	movq -144(%rbp), %r12	# Generic Register Translation
	movq %r11, 0(%r12)	# set size of dimension 1
	movq %r11, -96(%rbp)	# move result back to Generic Register
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -144(%rbp), %r11	# Generic Register Translation
	movq -152(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# initialize counter for initialization loop
	movq %r11, -144(%rbp)	# move result back to Generic Register
	movq %r12, -152(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r11	# Generic Register Translation
	movq -144(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Store end of array
	movq %r11, -136(%rbp)	# move result back to Generic Register
	movq %r12, -144(%rbp)	# move result back to Generic Register

L1_array_init_loop:
	movq -152(%rbp), %r12	# Generic Register Translation
	movq $0, (%r12)	# Set array element value to 0
	movq %r12, -152(%rbp)	# move result back to Generic Register
	movq -152(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# increment counter
	movq %r12, -152(%rbp)	# move result back to Generic Register
	movq -144(%rbp), %r11	# Generic Register Translation
	movq -152(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12	# check if we are done initializing
	movq %r11, -144(%rbp)	# move result back to Generic Register
	movq %r12, -152(%rbp)	# move result back to Generic Register
	jg L1_array_init_loop	# if we are done initializing, jump to end of initialization loop
	movq -136(%rbp), %r11	# Generic Register Translation
	movq -144(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12	# restore end of array to be start of array
	movq %r11, -136(%rbp)	# move result back to Generic Register
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -144(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# set array pointer to point to first element
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -144(%rbp), %r11	# Generic Register Translation
	movq -160(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -144(%rbp)	# move result back to Generic Register
	movq %r12, -160(%rbp)	# move result back to Generic Register
	movq -168(%rbp), %r12	# Generic Register Translation
	movq $8, %r12	# initialize memory size
	movq %r12, -168(%rbp)	# move result back to Generic Register
	movq -128(%rbp), %r11	# Generic Register Translation
	movq -168(%rbp), %r12	# Generic Register Translation
	imulq %r11, %r12	# calculate memory
	movq %r11, -128(%rbp)	# move result back to Generic Register
	movq %r12, -168(%rbp)	# move result back to Generic Register
	movq -168(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# found size of memory
	movq %r12, -168(%rbp)	# move result back to Generic Register
	movq -168(%rbp), %r11	# Generic Register Translation
	movq %r11, %rdi	# set memory size for allocation
	movq %r11, -168(%rbp)	# move result back to Generic Register
	call allocate	# allocate memory of found memory size
	movq -176(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# Save array pointer in generic register
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -128(%rbp), %r11	# Generic Register Translation
	movq -176(%rbp), %r12	# Generic Register Translation
	movq %r11, 0(%r12)	# set size of dimension 1
	movq %r11, -128(%rbp)	# move result back to Generic Register
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r11	# Generic Register Translation
	movq -184(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# initialize counter for initialization loop
	movq %r11, -176(%rbp)	# move result back to Generic Register
	movq %r12, -184(%rbp)	# move result back to Generic Register
	movq -168(%rbp), %r11	# Generic Register Translation
	movq -176(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Store end of array
	movq %r11, -168(%rbp)	# move result back to Generic Register
	movq %r12, -176(%rbp)	# move result back to Generic Register

L2_array_init_loop:
	movq -184(%rbp), %r12	# Generic Register Translation
	movq $0, (%r12)	# Set array element value to 0
	movq %r12, -184(%rbp)	# move result back to Generic Register
	movq -184(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# increment counter
	movq %r12, -184(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r11	# Generic Register Translation
	movq -184(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12	# check if we are done initializing
	movq %r11, -176(%rbp)	# move result back to Generic Register
	movq %r12, -184(%rbp)	# move result back to Generic Register
	jg L2_array_init_loop	# if we are done initializing, jump to end of initialization loop
	movq -168(%rbp), %r11	# Generic Register Translation
	movq -176(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12	# restore end of array to be start of array
	movq %r11, -168(%rbp)	# move result back to Generic Register
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# set array pointer to point to first element
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r11	# Generic Register Translation
	movq -192(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -176(%rbp)	# move result back to Generic Register
	movq %r12, -192(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r12	# Generic Register Translation
	movq $0, %r12	# assign value to local variable
	movq %r12, -200(%rbp)	# move result back to Generic Register

L3_while_statement:
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -208(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -208(%rbp)	# move result back to Generic Register
	movq -96(%rbp), %r11	# Generic Register Translation
	movq -216(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -96(%rbp)	# move result back to Generic Register
	movq %r12, -216(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -216(%rbp), %r11	# Generic Register Translation
	movq -208(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -216(%rbp)	# move result back to Generic Register
	movq %r12, -208(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -224(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -224(%rbp)	# move result back to Generic Register
	movq -224(%rbp), %r11	# Generic Register Translation
	movq -232(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -224(%rbp)	# move result back to Generic Register
	movq %r12, -232(%rbp)	# move result back to Generic Register
	movq -232(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -232(%rbp)	# move result back to Generic Register
	jne L4_end_while_statement
	movq -160(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -160(%rbp)	# move result back to Generic Register
	jne L5_after_beta_check
	movq $10, %rdi	# Line number
	call print_is_beta

L5_after_beta_check:
	movq -160(%rbp), %r11	# Generic Register Translation
	movq -240(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -160(%rbp)	# move result back to Generic Register
	movq %r12, -240(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -248(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -248(%rbp)	# move result back to Generic Register
	movq -248(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -248(%rbp)	# move result back to Generic Register
	movq -248(%rbp), %r11	# Generic Register Translation
	movq -240(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -248(%rbp)	# move result back to Generic Register
	movq %r12, -240(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -256(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -256(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -264(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -264(%rbp)	# move result back to Generic Register
	movq -264(%rbp), %r11	# Generic Register Translation
	movq -256(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -264(%rbp)	# move result back to Generic Register
	movq %r12, -256(%rbp)	# move result back to Generic Register
	movq -256(%rbp), %r11	# Generic Register Translation
	movq -272(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -256(%rbp)	# move result back to Generic Register
	movq %r12, -272(%rbp)	# move result back to Generic Register
	movq 24(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, 24(%rbp)	# move result back to Generic Register
	jne L6_after_beta_check
	movq $10, %rdi	# Line number
	call print_is_beta

L6_after_beta_check:
	movq 24(%rbp), %r11	# Generic Register Translation
	movq -280(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq %r12, -280(%rbp)	# move result back to Generic Register
	movq -272(%rbp), %r11	# Generic Register Translation
	movq -288(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -272(%rbp)	# move result back to Generic Register
	movq %r12, -288(%rbp)	# move result back to Generic Register
	movq -288(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -288(%rbp)	# move result back to Generic Register
	movq -288(%rbp), %r11	# Generic Register Translation
	movq -280(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -288(%rbp)	# move result back to Generic Register
	movq %r12, -280(%rbp)	# move result back to Generic Register
	movq -280(%rbp), %r11	# Generic Register Translation
	movq -280(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -280(%rbp)	# move result back to Generic Register
	movq %r12, -280(%rbp)	# move result back to Generic Register
	movq -280(%rbp), %r11	# Generic Register Translation
	movq -240(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -280(%rbp)	# move result back to Generic Register
	movq %r12, -240(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -296(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -296(%rbp)	# move result back to Generic Register
	movq -304(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -304(%rbp)	# move result back to Generic Register
	movq -304(%rbp), %r11	# Generic Register Translation
	movq -296(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -304(%rbp)	# move result back to Generic Register
	movq %r12, -296(%rbp)	# move result back to Generic Register
	movq -296(%rbp), %r11	# Generic Register Translation
	movq -312(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -296(%rbp)	# move result back to Generic Register
	movq %r12, -312(%rbp)	# move result back to Generic Register
	movq -312(%rbp), %r11	# Generic Register Translation
	movq -200(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -312(%rbp)	# move result back to Generic Register
	movq %r12, -200(%rbp)	# move result back to Generic Register
	jmp L3_while_statement

L4_end_while_statement:
	movq -320(%rbp), %r12	# Generic Register Translation
	movq $0, %r12	# assign value to local variable
	movq %r12, -320(%rbp)	# move result back to Generic Register

L7_while_statement:
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -328(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -328(%rbp)	# move result back to Generic Register
	movq -128(%rbp), %r11	# Generic Register Translation
	movq -336(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -128(%rbp)	# move result back to Generic Register
	movq %r12, -336(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -336(%rbp), %r11	# Generic Register Translation
	movq -328(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -336(%rbp)	# move result back to Generic Register
	movq %r12, -328(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -344(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -344(%rbp)	# move result back to Generic Register
	movq -344(%rbp), %r11	# Generic Register Translation
	movq -352(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -344(%rbp)	# move result back to Generic Register
	movq %r12, -352(%rbp)	# move result back to Generic Register
	movq -352(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -352(%rbp)	# move result back to Generic Register
	jne L8_end_while_statement
	movq -192(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -192(%rbp)	# move result back to Generic Register
	jne L9_after_beta_check
	movq $16, %rdi	# Line number
	call print_is_beta

L9_after_beta_check:
	movq -192(%rbp), %r11	# Generic Register Translation
	movq -360(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -192(%rbp)	# move result back to Generic Register
	movq %r12, -360(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -368(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -368(%rbp)	# move result back to Generic Register
	movq -368(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -368(%rbp)	# move result back to Generic Register
	movq -368(%rbp), %r11	# Generic Register Translation
	movq -360(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -368(%rbp)	# move result back to Generic Register
	movq %r12, -360(%rbp)	# move result back to Generic Register
	movq 40(%rbp), %r11	# Generic Register Translation
	movq -376(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq %r12, -376(%rbp)	# move result back to Generic Register
	movq -384(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -384(%rbp)	# move result back to Generic Register
	movq -384(%rbp), %r11	# Generic Register Translation
	movq -376(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -384(%rbp)	# move result back to Generic Register
	movq %r12, -376(%rbp)	# move result back to Generic Register
	movq -376(%rbp), %r11	# Generic Register Translation
	movq -392(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -376(%rbp)	# move result back to Generic Register
	movq %r12, -392(%rbp)	# move result back to Generic Register
	movq -392(%rbp), %r11	# Generic Register Translation
	movq -400(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -392(%rbp)	# move result back to Generic Register
	movq %r12, -400(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -408(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -408(%rbp)	# move result back to Generic Register
	movq -408(%rbp), %r11	# Generic Register Translation
	movq -400(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -408(%rbp)	# move result back to Generic Register
	movq %r12, -400(%rbp)	# move result back to Generic Register
	movq -400(%rbp), %r11	# Generic Register Translation
	movq -416(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -400(%rbp)	# move result back to Generic Register
	movq %r12, -416(%rbp)	# move result back to Generic Register
	movq 24(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, 24(%rbp)	# move result back to Generic Register
	jne L10_after_beta_check
	movq $16, %rdi	# Line number
	call print_is_beta

L10_after_beta_check:
	movq 24(%rbp), %r11	# Generic Register Translation
	movq -424(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq %r12, -424(%rbp)	# move result back to Generic Register
	movq -416(%rbp), %r11	# Generic Register Translation
	movq -432(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -416(%rbp)	# move result back to Generic Register
	movq %r12, -432(%rbp)	# move result back to Generic Register
	movq -432(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -432(%rbp)	# move result back to Generic Register
	movq -432(%rbp), %r11	# Generic Register Translation
	movq -424(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -432(%rbp)	# move result back to Generic Register
	movq %r12, -424(%rbp)	# move result back to Generic Register
	movq -424(%rbp), %r11	# Generic Register Translation
	movq -424(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -424(%rbp)	# move result back to Generic Register
	movq %r12, -424(%rbp)	# move result back to Generic Register
	movq -424(%rbp), %r11	# Generic Register Translation
	movq -360(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -424(%rbp)	# move result back to Generic Register
	movq %r12, -360(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -440(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -440(%rbp)	# move result back to Generic Register
	movq -448(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -448(%rbp)	# move result back to Generic Register
	movq -448(%rbp), %r11	# Generic Register Translation
	movq -440(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -448(%rbp)	# move result back to Generic Register
	movq %r12, -440(%rbp)	# move result back to Generic Register
	movq -440(%rbp), %r11	# Generic Register Translation
	movq -456(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -440(%rbp)	# move result back to Generic Register
	movq %r12, -456(%rbp)	# move result back to Generic Register
	movq -456(%rbp), %r11	# Generic Register Translation
	movq -320(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -456(%rbp)	# move result back to Generic Register
	movq %r12, -320(%rbp)	# move result back to Generic Register
	jmp L7_while_statement

L8_end_while_statement:
	movq -200(%rbp), %r12	# Generic Register Translation
	movq $0, %r12	# assign value to local variable
	movq %r12, -200(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r12	# Generic Register Translation
	movq $0, %r12	# assign value to local variable
	movq %r12, -320(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -464(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -464(%rbp)	# move result back to Generic Register

L11_while_statement:
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -472(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -472(%rbp)	# move result back to Generic Register
	movq -96(%rbp), %r11	# Generic Register Translation
	movq -480(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -96(%rbp)	# move result back to Generic Register
	movq %r12, -480(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -480(%rbp), %r11	# Generic Register Translation
	movq -472(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -480(%rbp)	# move result back to Generic Register
	movq %r12, -472(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -488(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -488(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -496(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -496(%rbp)	# move result back to Generic Register
	movq -128(%rbp), %r11	# Generic Register Translation
	movq -504(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -128(%rbp)	# move result back to Generic Register
	movq %r12, -504(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -504(%rbp), %r11	# Generic Register Translation
	movq -496(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -504(%rbp)	# move result back to Generic Register
	movq %r12, -496(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -512(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -512(%rbp)	# move result back to Generic Register
	movq -488(%rbp), %r11	# Generic Register Translation
	movq -520(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -488(%rbp)	# move result back to Generic Register
	movq %r12, -520(%rbp)	# move result back to Generic Register
	movq -512(%rbp), %r11	# Generic Register Translation
	movq -528(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -512(%rbp)	# move result back to Generic Register
	movq %r12, -528(%rbp)	# move result back to Generic Register
	movq -528(%rbp), %r11	# Generic Register Translation
	movq -520(%rbp), %r12	# Generic Register Translation
	andq %r11, %r12
	movq %r11, -528(%rbp)	# move result back to Generic Register
	movq %r12, -520(%rbp)	# move result back to Generic Register
	movq -520(%rbp), %r11	# Generic Register Translation
	movq -536(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -520(%rbp)	# move result back to Generic Register
	movq %r12, -536(%rbp)	# move result back to Generic Register
	movq -536(%rbp), %r11	# Generic Register Translation
	movq -544(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -536(%rbp)	# move result back to Generic Register
	movq %r12, -544(%rbp)	# move result back to Generic Register
	movq -544(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -544(%rbp)	# move result back to Generic Register
	jne L12_end_while_statement

L19_if_statement:
	movq -160(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -160(%rbp)	# move result back to Generic Register
	jne L13_after_beta_check
	movq $25, %rdi	# Line number
	call print_is_beta

L13_after_beta_check:
	movq -160(%rbp), %r11	# Generic Register Translation
	movq -552(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -160(%rbp)	# move result back to Generic Register
	movq %r12, -552(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -560(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -560(%rbp)	# move result back to Generic Register
	movq -560(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -560(%rbp)	# move result back to Generic Register
	movq -560(%rbp), %r11	# Generic Register Translation
	movq -552(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -560(%rbp)	# move result back to Generic Register
	movq %r12, -552(%rbp)	# move result back to Generic Register
	movq -552(%rbp), %r11	# Generic Register Translation
	movq -552(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -552(%rbp)	# move result back to Generic Register
	movq %r12, -552(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -192(%rbp)	# move result back to Generic Register
	jne L14_after_beta_check
	movq $25, %rdi	# Line number
	call print_is_beta

L14_after_beta_check:
	movq -192(%rbp), %r11	# Generic Register Translation
	movq -568(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -192(%rbp)	# move result back to Generic Register
	movq %r12, -568(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -576(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -576(%rbp)	# move result back to Generic Register
	movq -576(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -576(%rbp)	# move result back to Generic Register
	movq -576(%rbp), %r11	# Generic Register Translation
	movq -568(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -576(%rbp)	# move result back to Generic Register
	movq %r12, -568(%rbp)	# move result back to Generic Register
	movq -568(%rbp), %r11	# Generic Register Translation
	movq -568(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -568(%rbp)	# move result back to Generic Register
	movq %r12, -568(%rbp)	# move result back to Generic Register
	movq -552(%rbp), %r11	# Generic Register Translation
	movq -584(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -552(%rbp)	# move result back to Generic Register
	movq %r12, -584(%rbp)	# move result back to Generic Register
	movq -568(%rbp), %r11	# Generic Register Translation
	movq -592(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -568(%rbp)	# move result back to Generic Register
	movq %r12, -592(%rbp)	# move result back to Generic Register
	xorq %r10, %r10	# start of less than or equal compare
	movq -592(%rbp), %r11	# Generic Register Translation
	movq -584(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -592(%rbp)	# move result back to Generic Register
	movq %r12, -584(%rbp)	# move result back to Generic Register
	setle %r10b
	movq -600(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -600(%rbp)	# move result back to Generic Register
	movq -600(%rbp), %r11	# Generic Register Translation
	movq -608(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -600(%rbp)	# move result back to Generic Register
	movq %r12, -608(%rbp)	# move result back to Generic Register
	movq -608(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -608(%rbp)	# move result back to Generic Register
	jne L21_else_statement
	movq 24(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, 24(%rbp)	# move result back to Generic Register
	jne L15_after_beta_check
	movq $26, %rdi	# Line number
	call print_is_beta

L15_after_beta_check:
	movq 24(%rbp), %r11	# Generic Register Translation
	movq -616(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq %r12, -616(%rbp)	# move result back to Generic Register
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -624(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -624(%rbp)	# move result back to Generic Register
	movq -624(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -624(%rbp)	# move result back to Generic Register
	movq -624(%rbp), %r11	# Generic Register Translation
	movq -616(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -624(%rbp)	# move result back to Generic Register
	movq %r12, -616(%rbp)	# move result back to Generic Register
	movq -160(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -160(%rbp)	# move result back to Generic Register
	jne L16_after_beta_check
	movq $26, %rdi	# Line number
	call print_is_beta

L16_after_beta_check:
	movq -160(%rbp), %r11	# Generic Register Translation
	movq -632(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -160(%rbp)	# move result back to Generic Register
	movq %r12, -632(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -640(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -640(%rbp)	# move result back to Generic Register
	movq -640(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -640(%rbp)	# move result back to Generic Register
	movq -640(%rbp), %r11	# Generic Register Translation
	movq -632(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -640(%rbp)	# move result back to Generic Register
	movq %r12, -632(%rbp)	# move result back to Generic Register
	movq -632(%rbp), %r11	# Generic Register Translation
	movq -632(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -632(%rbp)	# move result back to Generic Register
	movq %r12, -632(%rbp)	# move result back to Generic Register
	movq -632(%rbp), %r11	# Generic Register Translation
	movq -616(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -632(%rbp)	# move result back to Generic Register
	movq %r12, -616(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -648(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -648(%rbp)	# move result back to Generic Register
	movq -656(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -656(%rbp)	# move result back to Generic Register
	movq -656(%rbp), %r11	# Generic Register Translation
	movq -648(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -656(%rbp)	# move result back to Generic Register
	movq %r12, -648(%rbp)	# move result back to Generic Register
	movq -648(%rbp), %r11	# Generic Register Translation
	movq -664(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -648(%rbp)	# move result back to Generic Register
	movq %r12, -664(%rbp)	# move result back to Generic Register
	movq -664(%rbp), %r11	# Generic Register Translation
	movq -200(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -664(%rbp)	# move result back to Generic Register
	movq %r12, -200(%rbp)	# move result back to Generic Register
	jmp L20_endif_statement

L21_else_statement:
	movq 24(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, 24(%rbp)	# move result back to Generic Register
	jne L17_after_beta_check
	movq $29, %rdi	# Line number
	call print_is_beta

L17_after_beta_check:
	movq 24(%rbp), %r11	# Generic Register Translation
	movq -672(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq %r12, -672(%rbp)	# move result back to Generic Register
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -680(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -680(%rbp)	# move result back to Generic Register
	movq -680(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -680(%rbp)	# move result back to Generic Register
	movq -680(%rbp), %r11	# Generic Register Translation
	movq -672(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -680(%rbp)	# move result back to Generic Register
	movq %r12, -672(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -192(%rbp)	# move result back to Generic Register
	jne L18_after_beta_check
	movq $29, %rdi	# Line number
	call print_is_beta

L18_after_beta_check:
	movq -192(%rbp), %r11	# Generic Register Translation
	movq -688(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -192(%rbp)	# move result back to Generic Register
	movq %r12, -688(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -696(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -696(%rbp)	# move result back to Generic Register
	movq -696(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -696(%rbp)	# move result back to Generic Register
	movq -696(%rbp), %r11	# Generic Register Translation
	movq -688(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -696(%rbp)	# move result back to Generic Register
	movq %r12, -688(%rbp)	# move result back to Generic Register
	movq -688(%rbp), %r11	# Generic Register Translation
	movq -688(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -688(%rbp)	# move result back to Generic Register
	movq %r12, -688(%rbp)	# move result back to Generic Register
	movq -688(%rbp), %r11	# Generic Register Translation
	movq -672(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -688(%rbp)	# move result back to Generic Register
	movq %r12, -672(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -704(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -704(%rbp)	# move result back to Generic Register
	movq -712(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -712(%rbp)	# move result back to Generic Register
	movq -712(%rbp), %r11	# Generic Register Translation
	movq -704(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -712(%rbp)	# move result back to Generic Register
	movq %r12, -704(%rbp)	# move result back to Generic Register
	movq -704(%rbp), %r11	# Generic Register Translation
	movq -720(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -704(%rbp)	# move result back to Generic Register
	movq %r12, -720(%rbp)	# move result back to Generic Register
	movq -720(%rbp), %r11	# Generic Register Translation
	movq -320(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -720(%rbp)	# move result back to Generic Register
	movq %r12, -320(%rbp)	# move result back to Generic Register

L20_endif_statement:
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -728(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -728(%rbp)	# move result back to Generic Register
	movq -736(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -736(%rbp)	# move result back to Generic Register
	movq -736(%rbp), %r11	# Generic Register Translation
	movq -728(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -736(%rbp)	# move result back to Generic Register
	movq %r12, -728(%rbp)	# move result back to Generic Register
	movq -728(%rbp), %r11	# Generic Register Translation
	movq -744(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -728(%rbp)	# move result back to Generic Register
	movq %r12, -744(%rbp)	# move result back to Generic Register
	movq -744(%rbp), %r11	# Generic Register Translation
	movq -464(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -744(%rbp)	# move result back to Generic Register
	movq %r12, -464(%rbp)	# move result back to Generic Register
	jmp L11_while_statement

L12_end_while_statement:

L22_while_statement:
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -752(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -752(%rbp)	# move result back to Generic Register
	movq -96(%rbp), %r11	# Generic Register Translation
	movq -760(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -96(%rbp)	# move result back to Generic Register
	movq %r12, -760(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -760(%rbp), %r11	# Generic Register Translation
	movq -752(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -760(%rbp)	# move result back to Generic Register
	movq %r12, -752(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -768(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -768(%rbp)	# move result back to Generic Register
	movq -768(%rbp), %r11	# Generic Register Translation
	movq -776(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -768(%rbp)	# move result back to Generic Register
	movq %r12, -776(%rbp)	# move result back to Generic Register
	movq -776(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -776(%rbp)	# move result back to Generic Register
	jne L23_end_while_statement
	movq 24(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, 24(%rbp)	# move result back to Generic Register
	jne L24_after_beta_check
	movq $36, %rdi	# Line number
	call print_is_beta

L24_after_beta_check:
	movq 24(%rbp), %r11	# Generic Register Translation
	movq -784(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq %r12, -784(%rbp)	# move result back to Generic Register
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -792(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -792(%rbp)	# move result back to Generic Register
	movq -792(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -792(%rbp)	# move result back to Generic Register
	movq -792(%rbp), %r11	# Generic Register Translation
	movq -784(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -792(%rbp)	# move result back to Generic Register
	movq %r12, -784(%rbp)	# move result back to Generic Register
	movq -160(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -160(%rbp)	# move result back to Generic Register
	jne L25_after_beta_check
	movq $36, %rdi	# Line number
	call print_is_beta

L25_after_beta_check:
	movq -160(%rbp), %r11	# Generic Register Translation
	movq -800(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -160(%rbp)	# move result back to Generic Register
	movq %r12, -800(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -808(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -808(%rbp)	# move result back to Generic Register
	movq -808(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -808(%rbp)	# move result back to Generic Register
	movq -808(%rbp), %r11	# Generic Register Translation
	movq -800(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -808(%rbp)	# move result back to Generic Register
	movq %r12, -800(%rbp)	# move result back to Generic Register
	movq -800(%rbp), %r11	# Generic Register Translation
	movq -800(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -800(%rbp)	# move result back to Generic Register
	movq %r12, -800(%rbp)	# move result back to Generic Register
	movq -800(%rbp), %r11	# Generic Register Translation
	movq -784(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -800(%rbp)	# move result back to Generic Register
	movq %r12, -784(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -816(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -816(%rbp)	# move result back to Generic Register
	movq -824(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -824(%rbp)	# move result back to Generic Register
	movq -824(%rbp), %r11	# Generic Register Translation
	movq -816(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -824(%rbp)	# move result back to Generic Register
	movq %r12, -816(%rbp)	# move result back to Generic Register
	movq -816(%rbp), %r11	# Generic Register Translation
	movq -832(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -816(%rbp)	# move result back to Generic Register
	movq %r12, -832(%rbp)	# move result back to Generic Register
	movq -832(%rbp), %r11	# Generic Register Translation
	movq -200(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -832(%rbp)	# move result back to Generic Register
	movq %r12, -200(%rbp)	# move result back to Generic Register
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -840(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -840(%rbp)	# move result back to Generic Register
	movq -848(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -848(%rbp)	# move result back to Generic Register
	movq -848(%rbp), %r11	# Generic Register Translation
	movq -840(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -848(%rbp)	# move result back to Generic Register
	movq %r12, -840(%rbp)	# move result back to Generic Register
	movq -840(%rbp), %r11	# Generic Register Translation
	movq -856(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -840(%rbp)	# move result back to Generic Register
	movq %r12, -856(%rbp)	# move result back to Generic Register
	movq -856(%rbp), %r11	# Generic Register Translation
	movq -464(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -856(%rbp)	# move result back to Generic Register
	movq %r12, -464(%rbp)	# move result back to Generic Register
	jmp L22_while_statement

L23_end_while_statement:

L26_while_statement:
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -864(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -864(%rbp)	# move result back to Generic Register
	movq -128(%rbp), %r11	# Generic Register Translation
	movq -872(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -128(%rbp)	# move result back to Generic Register
	movq %r12, -872(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -872(%rbp), %r11	# Generic Register Translation
	movq -864(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -872(%rbp)	# move result back to Generic Register
	movq %r12, -864(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -880(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -880(%rbp)	# move result back to Generic Register
	movq -880(%rbp), %r11	# Generic Register Translation
	movq -888(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -880(%rbp)	# move result back to Generic Register
	movq %r12, -888(%rbp)	# move result back to Generic Register
	movq -888(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -888(%rbp)	# move result back to Generic Register
	jne L27_end_while_statement
	movq 24(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, 24(%rbp)	# move result back to Generic Register
	jne L28_after_beta_check
	movq $42, %rdi	# Line number
	call print_is_beta

L28_after_beta_check:
	movq 24(%rbp), %r11	# Generic Register Translation
	movq -896(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq %r12, -896(%rbp)	# move result back to Generic Register
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -904(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -904(%rbp)	# move result back to Generic Register
	movq -904(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -904(%rbp)	# move result back to Generic Register
	movq -904(%rbp), %r11	# Generic Register Translation
	movq -896(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -904(%rbp)	# move result back to Generic Register
	movq %r12, -896(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -192(%rbp)	# move result back to Generic Register
	jne L29_after_beta_check
	movq $42, %rdi	# Line number
	call print_is_beta

L29_after_beta_check:
	movq -192(%rbp), %r11	# Generic Register Translation
	movq -912(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -192(%rbp)	# move result back to Generic Register
	movq %r12, -912(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -920(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -920(%rbp)	# move result back to Generic Register
	movq -920(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -920(%rbp)	# move result back to Generic Register
	movq -920(%rbp), %r11	# Generic Register Translation
	movq -912(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -920(%rbp)	# move result back to Generic Register
	movq %r12, -912(%rbp)	# move result back to Generic Register
	movq -912(%rbp), %r11	# Generic Register Translation
	movq -912(%rbp), %r12	# Generic Register Translation
	movq (%r11), %r12	# Unwrap the index pointer
	movq %r11, -912(%rbp)	# move result back to Generic Register
	movq %r12, -912(%rbp)	# move result back to Generic Register
	movq -912(%rbp), %r11	# Generic Register Translation
	movq -896(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -912(%rbp)	# move result back to Generic Register
	movq %r12, -896(%rbp)	# move result back to Generic Register
	movq -320(%rbp), %r11	# Generic Register Translation
	movq -928(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -320(%rbp)	# move result back to Generic Register
	movq %r12, -928(%rbp)	# move result back to Generic Register
	movq -936(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -936(%rbp)	# move result back to Generic Register
	movq -936(%rbp), %r11	# Generic Register Translation
	movq -928(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -936(%rbp)	# move result back to Generic Register
	movq %r12, -928(%rbp)	# move result back to Generic Register
	movq -928(%rbp), %r11	# Generic Register Translation
	movq -944(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -928(%rbp)	# move result back to Generic Register
	movq %r12, -944(%rbp)	# move result back to Generic Register
	movq -944(%rbp), %r11	# Generic Register Translation
	movq -320(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -944(%rbp)	# move result back to Generic Register
	movq %r12, -320(%rbp)	# move result back to Generic Register
	movq -464(%rbp), %r11	# Generic Register Translation
	movq -952(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -464(%rbp)	# move result back to Generic Register
	movq %r12, -952(%rbp)	# move result back to Generic Register
	movq -960(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -960(%rbp)	# move result back to Generic Register
	movq -960(%rbp), %r11	# Generic Register Translation
	movq -952(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -960(%rbp)	# move result back to Generic Register
	movq %r12, -952(%rbp)	# move result back to Generic Register
	movq -952(%rbp), %r11	# Generic Register Translation
	movq -968(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -952(%rbp)	# move result back to Generic Register
	movq %r12, -968(%rbp)	# move result back to Generic Register
	movq -968(%rbp), %r11	# Generic Register Translation
	movq -464(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -968(%rbp)	# move result back to Generic Register
	movq %r12, -464(%rbp)	# move result back to Generic Register
	jmp L26_while_statement

L27_end_while_statement:
	movq $0, %rax
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

L30_mergeSortHelper:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0

L31_while_statement:
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	movq 40(%rbp), %r11	# Generic Register Translation
	movq -56(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq %r12, -56(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -56(%rbp), %r11	# Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -56(%rbp)	# move result back to Generic Register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -64(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -64(%rbp)	# move result back to Generic Register
	movq -64(%rbp), %r11	# Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -64(%rbp)	# move result back to Generic Register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -72(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -72(%rbp)	# move result back to Generic Register
	jne L32_end_while_statement
	movq 40(%rbp), %r11	# Generic Register Translation
	movq -80(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq %r12, -80(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -88(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -88(%rbp)	# move result back to Generic Register
	movq -88(%rbp), %r11	# Generic Register Translation
	movq -80(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12
	movq %r11, -88(%rbp)	# move result back to Generic Register
	movq %r12, -80(%rbp)	# move result back to Generic Register
	movq -80(%rbp), %r11	# Generic Register Translation
	movq -96(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -80(%rbp)	# move result back to Generic Register
	movq %r12, -96(%rbp)	# move result back to Generic Register
	movq -96(%rbp), %r11	# Generic Register Translation
	movq -104(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -96(%rbp)	# move result back to Generic Register
	movq %r12, -104(%rbp)	# move result back to Generic Register
	movq -112(%rbp), %r12	# Generic Register Translation
	movq $2, %r12
	movq %r12, -112(%rbp)	# move result back to Generic Register
	xorq %rdx, %rdx
	movq -104(%rbp), %r11	# Generic Register Translation
	movq %r11, %rax
	movq %r11, -104(%rbp)	# move result back to Generic Register
	movq -112(%rbp), %r11	# Generic Register Translation
	idivq %r11
	movq %r11, -112(%rbp)	# move result back to Generic Register
	movq -120(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12
	movq %r12, -120(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -128(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -128(%rbp)	# move result back to Generic Register
	movq -120(%rbp), %r11	# Generic Register Translation
	movq -136(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -120(%rbp)	# move result back to Generic Register
	movq %r12, -136(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r11	# Generic Register Translation
	movq -128(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -136(%rbp)	# move result back to Generic Register
	movq %r12, -128(%rbp)	# move result back to Generic Register
	movq -128(%rbp), %r11	# Generic Register Translation
	movq -144(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -128(%rbp)	# move result back to Generic Register
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -144(%rbp), %r11	# Generic Register Translation
	movq -152(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -144(%rbp)	# move result back to Generic Register
	movq %r12, -152(%rbp)	# move result back to Generic Register
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
	movq -152(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -152(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq 24(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq -160(%rbp), %r12	# Generic Register Translation
	movq %rbp, %r12	# calculating static link for function call
	movq %r12, -160(%rbp)	# move result back to Generic Register
	movq -160(%rbp), %r11	# Generic Register Translation
	movq -160(%rbp), %r12	# Generic Register Translation
	movq 16(%r11), %r12
	movq %r11, -160(%rbp)	# move result back to Generic Register
	movq %r12, -160(%rbp)	# move result back to Generic Register
	movq -160(%rbp), %r11	# Generic Register Translation
	pushq %r11	# setting static link
	movq %r11, -160(%rbp)	# move result back to Generic Register
	call L30_mergeSortHelper
	addq $32, %rsp	# remove arguments and static link from stack
	movq -168(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -168(%rbp)	# move result back to Generic Register
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
	movq -152(%rbp), %r11	# Generic Register Translation
	movq -176(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -152(%rbp)	# move result back to Generic Register
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -184(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -184(%rbp)	# move result back to Generic Register
	movq -184(%rbp), %r11	# Generic Register Translation
	movq -176(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -184(%rbp)	# move result back to Generic Register
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r11	# Generic Register Translation
	movq -192(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -176(%rbp)	# move result back to Generic Register
	movq %r12, -192(%rbp)	# move result back to Generic Register
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
	movq 40(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -192(%rbp)	# move result back to Generic Register
	movq 24(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r12	# Generic Register Translation
	movq %rbp, %r12	# calculating static link for function call
	movq %r12, -200(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	movq -200(%rbp), %r12	# Generic Register Translation
	movq 16(%r11), %r12
	movq %r11, -200(%rbp)	# move result back to Generic Register
	movq %r12, -200(%rbp)	# move result back to Generic Register
	movq -200(%rbp), %r11	# Generic Register Translation
	pushq %r11	# setting static link
	movq %r11, -200(%rbp)	# move result back to Generic Register
	call L30_mergeSortHelper
	addq $32, %rsp	# remove arguments and static link from stack
	movq -208(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -208(%rbp)	# move result back to Generic Register
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
	movq 40(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 40(%rbp)	# move result back to Generic Register
	movq -152(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -152(%rbp)	# move result back to Generic Register
	movq 32(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq 24(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq -216(%rbp), %r12	# Generic Register Translation
	movq %rbp, %r12	# calculating static link for function call
	movq %r12, -216(%rbp)	# move result back to Generic Register
	movq -216(%rbp), %r11	# Generic Register Translation
	movq -216(%rbp), %r12	# Generic Register Translation
	movq 16(%r11), %r12
	movq %r11, -216(%rbp)	# move result back to Generic Register
	movq %r12, -216(%rbp)	# move result back to Generic Register
	movq -216(%rbp), %r11	# Generic Register Translation
	pushq %r11	# setting static link
	movq %r11, -216(%rbp)	# move result back to Generic Register
	call L0_merge
	addq $40, %rsp	# remove arguments and static link from stack
	movq -224(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -224(%rbp)	# move result back to Generic Register
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
	movq $0, %rax
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
	jmp L31_while_statement

L32_end_while_statement:
	movq $0, %rax
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

L33_mergeSort:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	movq 32(%rbp), %r11	# Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, 32(%rbp)	# move result back to Generic Register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -56(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r11	# Generic Register Translation
	movq -48(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12
	movq %r11, -56(%rbp)	# move result back to Generic Register
	movq %r12, -48(%rbp)	# move result back to Generic Register
	movq -48(%rbp), %r11	# Generic Register Translation
	movq -64(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq %r12, -64(%rbp)	# move result back to Generic Register
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
	movq -64(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -64(%rbp)	# move result back to Generic Register
	pushq $0	# pushing register argument
	movq 24(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, 24(%rbp)	# move result back to Generic Register
	movq -72(%rbp), %r12	# Generic Register Translation
	movq %rbp, %r12	# calculating static link for function call
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -72(%rbp), %r11	# Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	movq 16(%r11), %r12
	movq %r11, -72(%rbp)	# move result back to Generic Register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -72(%rbp), %r11	# Generic Register Translation
	pushq %r11	# setting static link
	movq %r11, -72(%rbp)	# move result back to Generic Register
	call L30_mergeSortHelper
	addq $32, %rsp	# remove arguments and static link from stack
	movq -80(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -80(%rbp)	# move result back to Generic Register
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
	movq -80(%rbp), %r11	# Generic Register Translation
	movq %r11, %rax
	movq %r11, -80(%rbp)	# move result back to Generic Register
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

main:
	pushq %rbp	# save old rbp
	movq %rsp, %rbp	# set rbp for function scope
	pushq %rbx	# save RBX
	pushq %r12	# save R12
	pushq %r13	# save R13
	pushq %r14	# save R14
	pushq %r15	# save R15
	# END OF CALLEE SAVE
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	pushq $0	# Setting temporary variable to 0
	movq -48(%rbp), %r12	# Generic Register Translation
	movq $4000000, %r12	# assign value to local variable
	movq %r12, -48(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r12	# Generic Register Translation
	movq $8, %r12	# initialize memory size
	movq %r12, -56(%rbp)	# move result back to Generic Register
	movq -48(%rbp), %r11	# Generic Register Translation
	movq -56(%rbp), %r12	# Generic Register Translation
	imulq %r11, %r12	# calculate memory
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq %r12, -56(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# found size of memory
	movq %r12, -56(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r11	# Generic Register Translation
	movq %r11, %rdi	# set memory size for allocation
	movq %r11, -56(%rbp)	# move result back to Generic Register
	call allocate	# allocate memory of found memory size
	movq -64(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# Save array pointer in generic register
	movq %r12, -64(%rbp)	# move result back to Generic Register
	movq -48(%rbp), %r11	# Generic Register Translation
	movq -64(%rbp), %r12	# Generic Register Translation
	movq %r11, 0(%r12)	# set size of dimension 1
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq %r12, -64(%rbp)	# move result back to Generic Register
	movq -64(%rbp), %r11	# Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# initialize counter for initialization loop
	movq %r11, -64(%rbp)	# move result back to Generic Register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -56(%rbp), %r11	# Generic Register Translation
	movq -64(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Store end of array
	movq %r11, -56(%rbp)	# move result back to Generic Register
	movq %r12, -64(%rbp)	# move result back to Generic Register

L34_array_init_loop:
	movq -72(%rbp), %r12	# Generic Register Translation
	movq $0, (%r12)	# Set array element value to 0
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -72(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# increment counter
	movq %r12, -72(%rbp)	# move result back to Generic Register
	movq -64(%rbp), %r11	# Generic Register Translation
	movq -72(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12	# check if we are done initializing
	movq %r11, -64(%rbp)	# move result back to Generic Register
	movq %r12, -72(%rbp)	# move result back to Generic Register
	jg L34_array_init_loop	# if we are done initializing, jump to end of initialization loop
	movq -56(%rbp), %r11	# Generic Register Translation
	movq -64(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12	# restore end of array to be start of array
	movq %r11, -56(%rbp)	# move result back to Generic Register
	movq %r12, -64(%rbp)	# move result back to Generic Register
	movq -64(%rbp), %r12	# Generic Register Translation
	addq $8, %r12	# set array pointer to point to first element
	movq %r12, -64(%rbp)	# move result back to Generic Register
	movq -64(%rbp), %r11	# Generic Register Translation
	movq -80(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -64(%rbp)	# move result back to Generic Register
	movq %r12, -80(%rbp)	# move result back to Generic Register
	movq -88(%rbp), %r12	# Generic Register Translation
	movq $0, %r12	# assign value to local variable
	movq %r12, -88(%rbp)	# move result back to Generic Register

L35_while_statement:
	movq -88(%rbp), %r11	# Generic Register Translation
	movq -96(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -88(%rbp)	# move result back to Generic Register
	movq %r12, -96(%rbp)	# move result back to Generic Register
	movq -48(%rbp), %r11	# Generic Register Translation
	movq -104(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq %r12, -104(%rbp)	# move result back to Generic Register
	xorq %r10, %r10
	movq -104(%rbp), %r11	# Generic Register Translation
	movq -96(%rbp), %r12	# Generic Register Translation
	cmpq %r11, %r12
	movq %r11, -104(%rbp)	# move result back to Generic Register
	movq %r12, -96(%rbp)	# move result back to Generic Register
	setl %r10b
	movq -112(%rbp), %r12	# Generic Register Translation
	movq %r10, %r12
	movq %r12, -112(%rbp)	# move result back to Generic Register
	movq -112(%rbp), %r11	# Generic Register Translation
	movq -120(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -112(%rbp)	# move result back to Generic Register
	movq %r12, -120(%rbp)	# move result back to Generic Register
	movq -120(%rbp), %r12	# Generic Register Translation
	cmpq $1, %r12
	movq %r12, -120(%rbp)	# move result back to Generic Register
	jne L36_end_while_statement
	movq -80(%rbp), %r12	# Generic Register Translation
	cmpq $0, %r12	# Start checking for beta
	movq %r12, -80(%rbp)	# move result back to Generic Register
	jne L37_after_beta_check
	movq $75, %rdi	# Line number
	call print_is_beta

L37_after_beta_check:
	movq -80(%rbp), %r11	# Generic Register Translation
	movq -128(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the index address
	movq %r11, -80(%rbp)	# move result back to Generic Register
	movq %r12, -128(%rbp)	# move result back to Generic Register
	movq -88(%rbp), %r11	# Generic Register Translation
	movq -136(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# Initialize the intermediate value
	movq %r11, -88(%rbp)	# move result back to Generic Register
	movq %r12, -136(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r12	# Generic Register Translation
	imulq $8, %r12	# Translate the first index to byte address
	movq %r12, -136(%rbp)	# move result back to Generic Register
	movq -136(%rbp), %r11	# Generic Register Translation
	movq -128(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12	# Add the first index to the address
	movq %r11, -136(%rbp)	# move result back to Generic Register
	movq %r12, -128(%rbp)	# move result back to Generic Register
	movq -48(%rbp), %r11	# Generic Register Translation
	movq -144(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -88(%rbp), %r11	# Generic Register Translation
	movq -152(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -88(%rbp)	# move result back to Generic Register
	movq %r12, -152(%rbp)	# move result back to Generic Register
	movq -152(%rbp), %r11	# Generic Register Translation
	movq -144(%rbp), %r12	# Generic Register Translation
	subq %r11, %r12
	movq %r11, -152(%rbp)	# move result back to Generic Register
	movq %r12, -144(%rbp)	# move result back to Generic Register
	movq -144(%rbp), %r11	# Generic Register Translation
	movq -160(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -144(%rbp)	# move result back to Generic Register
	movq %r12, -160(%rbp)	# move result back to Generic Register
	movq -160(%rbp), %r11	# Generic Register Translation
	movq -128(%rbp), %r12	# Generic Register Translation
	movq %r11, (%r12)	# Assign the value to the array index
	movq %r11, -160(%rbp)	# move result back to Generic Register
	movq %r12, -128(%rbp)	# move result back to Generic Register
	movq -88(%rbp), %r11	# Generic Register Translation
	movq -168(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -88(%rbp)	# move result back to Generic Register
	movq %r12, -168(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r12	# Generic Register Translation
	movq $1, %r12
	movq %r12, -176(%rbp)	# move result back to Generic Register
	movq -176(%rbp), %r11	# Generic Register Translation
	movq -168(%rbp), %r12	# Generic Register Translation
	addq %r11, %r12
	movq %r11, -176(%rbp)	# move result back to Generic Register
	movq %r12, -168(%rbp)	# move result back to Generic Register
	movq -168(%rbp), %r11	# Generic Register Translation
	movq -184(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12
	movq %r11, -168(%rbp)	# move result back to Generic Register
	movq %r12, -184(%rbp)	# move result back to Generic Register
	movq -184(%rbp), %r11	# Generic Register Translation
	movq -88(%rbp), %r12	# Generic Register Translation
	movq %r11, %r12	# assign value to local variable
	movq %r11, -184(%rbp)	# move result back to Generic Register
	movq %r12, -88(%rbp)	# move result back to Generic Register
	jmp L35_while_statement

L36_end_while_statement:
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
	movq -48(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -48(%rbp)	# move result back to Generic Register
	movq -80(%rbp), %r11	# Generic Register Translation
	pushq %r11	# pushing register argument
	movq %r11, -80(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r12	# Generic Register Translation
	movq %rbp, %r12	# calculating static link for function call
	movq %r12, -192(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r11	# Generic Register Translation
	movq -192(%rbp), %r12	# Generic Register Translation
	movq 16(%r11), %r12
	movq %r11, -192(%rbp)	# move result back to Generic Register
	movq %r12, -192(%rbp)	# move result back to Generic Register
	movq -192(%rbp), %r11	# Generic Register Translation
	pushq %r11	# setting static link
	movq %r11, -192(%rbp)	# move result back to Generic Register
	call L33_mergeSort
	addq $24, %rsp	# remove arguments and static link from stack
	movq -200(%rbp), %r12	# Generic Register Translation
	movq %rax, %r12	# save result from function call
	movq %r12, -200(%rbp)	# move result back to Generic Register
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
	movq $0, %rax
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
	popq %r15	# restore R15
	popq %r14	# restore R14
	popq %r13	# restore R13
	popq %r12	# restore R12
	popq %rbx	# restore RBX
	# END OF CALLEE restore
	ret

allocate:
	pushq %rbp
	movq %rsp, %rbp
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

