#include <asm.h>

ENTRY(inner_task_switch)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %new
    pushl %ebp
    movl %esp, %ebp
    call inner_task_switch
    popl %ebp
    popl %new
    popl %edi
    popl %esi
    popl %ebx
    ret
    
