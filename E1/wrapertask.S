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
    addl $8, %esp
    popl %edi
    popl %esi
    popl %ebx
    ret

