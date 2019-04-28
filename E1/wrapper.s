# 1 "wrapper.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "wrapper.S"
# 1 "include/asm.h" 1
# 2 "wrapper.S" 2

.globl write; .type write, @function; .align 0; write:
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
 test %eax, %eax
 js errore

 popl %ebp
 ret
errore:
    neg %eax
 mov %eax, errno
 mov $-1, %eax
 popl %ebp
 ret

.globl gettime; .type gettime, @function; .align 0; gettime:
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

 popl %ebp
 ret;

 .globl getpid; .type getpid, @function; .align 0; getpid:
 pushl %ebp
 movl %esp, %ebp
 movl $20, %eax
    pushl %ecx
 pushl %edx
 pushl $retgetpid

 pushl %ebp
 movl %esp, %ebp

 sysenter


retgetpid:

 popl %ebp
 popl 0x4(%esp)
 popl %edx
 popl %ecx

 popl %ebp
 ret


 .globl fork; .type fork, @function; .align 0; fork:
 pushl %ebp
 movl %esp, %ebp
 movl $2, %eax
 pushl %ecx
 pushl %edx
 pushl $retfork

 pushl %ebp
 movl %esp, %ebp

 sysenter

retfork:
 popl %ebp
 popl 0x4(%esp)
 popl %edx
 popl %ecx

 popl %ebp
 ret

.globl exit; .type exit, @function; .align 0; exit:
 pushl %ebp
 movl %esp, %ebp
 movl $1, %eax
 pushl %eax
 sysenter
 popl %ebp
 ret
