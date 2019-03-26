#include <asm.h>

ENTRY(inner_task_switch)
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	pushl %esi
	pushl %edi
	pushl $new
	pushl %ebp
	movl %esp, %ebp	
	call task_switch
new: 
	popl %ebp
	addl $4, %esp
	popl %edi
	popl %esi
	popl %ebx
	ret

