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
