#include <asm.h>

ENTRY(write)
	pushl %ebp
	movl %esp, %ebp	
	pushl %ebx
        movl 8(%ebp), %ebx
	movl 12(%ebp), %ecx
	movl 16(%ebp), %edx 
	movl $0x04, %eax
	pushl %ecx
	pushl %edx
	pushl $retorn
	pushl %ebp
	movl %esp, %ebp	
	sysenter
retorn:   
	popl %ebp
	addl $4, %esp
	popl %edx
	popl %ecx
	popl %ebx
	cmpl %eax, $0
	jle errore // %eax < 0
	//movl %ebp, %esp
	popl %ebp
	ret
errore:
    neg %eax
	mov %eax, errno
	mov $-1, %eax
	popl %ebp
	ret

ENTRY(gettime)
	pushl %ebp
	movl %esp, %ebp
	movl $0x0A, %eax
	pushl %ecx
	pushl %edx
	pushl returno
	pushl %ebp
	movl %esp, %ebp
	sysenter
returno:
    	popl %ebp
    	popl 0x04(%esp)
	popl %edx
	popl %ecx
	//movl %ebp, %esp
	popl %ebp
	ret;

	ENTRY(getpid)
	pushl %ebp
	movl %esp, %ebp
	movl $20, %eax
    pushl %ecx
	pushl %edx
	pushl $retgetpid
	/* Create a fake dynamic link to access previous values easily from the handler in system mode */
	pushl %ebp
	movl %esp, %ebp
	/* Generate the trap: sysenter. */
	sysenter


retgetpid:
	/* Pop EBP, the return address, EDX and ECX from the stack. */
	popl %ebp
	popl 0x4(%esp)
	popl %edx
	popl %ecx
	/* Continue after sysenter */
	popl %ebp
	ret


	ENTRY(fork)
	pushl %ebp
	movl %esp, %ebp
	movl $2, %eax
	pushl %ecx
	pushl %edx
	pushl $retfork
	/* Create a fake dynamic link to access previous values easily from the handler in system mode */
	pushl %ebp
	movl %esp, %ebp
	/* Generate the trap: sysenter. */
	sysenter

retfork: 
	popl %ebp
	popl 0x4(%esp)
	popl %edx
	popl %ecx
	/* Continue after sysenter */
	popl %ebp
	ret

ENTRY(exit) 
	pushl %ebp
	movl %esp, %ebp
	movl $1, %eax
	call sys_exit
	popl %ebp
	ret