#include <libc.h>

char buff[24];


void fork1() {
  int ret = fork();
  stats(getpid());
  if(ret == 0) {
    write(1, "\nentra a fork1 fill\n",22);
    stats(getpid());
  }
  if(ret > 0) {
    write(1, "\nentra a fork1 pare\n",22);
    stats(getpid());
  }
}

void escriu_int(int i) {
	char c;
	itoa(i, &c);
	write(1, &c, strlen(&c));
}

void stats(int pid) {
	struct stats state;
	get_stats(getpid(), &state);
	
	write(1, "\n Stats del proces", 19);
	escriu_int(pid);

	write(1, "\nUser Ticks: ", 13);
	escriu_int(state.user_ticks);

	write(1, "\nSystem Ticks: ", 15);
	escriu_int(state.system_ticks);

	write(1, "\nBlocked Ticks: ", 16);
	escriu_int(state.blocked_ticks);

	write(1, "\nReady Ticks: ", 14);
	escriu_int(state.ready_ticks);
}



int __attribute__ ((__section__(".text.main")))
main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

	
  set_sched_policy(1);
  fork1();
  exit();
     
  while(1) { }
}

