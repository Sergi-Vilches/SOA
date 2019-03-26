/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>
#include <errno.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1

//wrmsr(MSR_IA32_SYSENTER_CS, __KERNEL_CS, 0);
//wrmsr(MSR_IA32_SYSENTER_ESP, tss->esp1, 0);
//wrmsr(MSR_IA32_SYSENTER_EIP, (unsigned long) sysenter_entry, 0);

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}
extern int zeos_ticks;
int sys_gettime() {
	return zeos_ticks;
}
int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}
#define buff 512
int sys_write(int fd, char * buffer, int size)
{
	int ret;	
	int errorfd = check_fd(fd, ESCRIPTURA);
	if(errorfd != 0) return errorfd;

	//Check if *buffer is not null
  if(buffer == NULL) return -EFAULT; // EFAULT -> bad address

  //Check if size is >= 0
  if(size < 0) return -EINVAL; //EINVAL -> invalid argument

  //Copying data from user space to kernel space
  int bytes = size;
  char sys_buffer[buff];
  while(bytes > buff) {
	copy_from_user(buffer,sys_buffer,size);
	ret = sys_write_console(sys_buffer, buff);
	bytes -= ret;
	buffer += ret;
}
if (bytes >0) {
	 copy_from_user(buffer,sys_buffer,size);
	ret = sys_write_console(sys_buffer, buff);
	bytes -= ret;
}
 int result = (size - bytes);
return result;
  }

  //Implement the requested service. For the I/O system calls, this requires calling the device dependent routine
  


void sys_exit()
{  
}
