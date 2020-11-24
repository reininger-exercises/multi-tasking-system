	.globl running, scheduler, tswitch
tswitch:
SAVE:
	pushl %eax
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %ebp
	pushl %esi
	pushl %edi
	pushfl
	movl running, %ebx # ebx -> PROC
	movl %esp, 4(%ebx) # PORC.save_sp = esp

FIND:
	call scheduler

RESUME:
	movl running, %ebx # ebx -> PROC
	movl 4(%ebx), %esp # esp = PROC.saved_sp
	popfl
	popl %edi
	popl %esi
	popl %ebp
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	ret
