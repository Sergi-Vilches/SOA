#include <libc.h>

char buff[24];

void escriu_int(int i) {
	char c;
	itoa(i, &c);
	write(1, &c, strlen(&c));
}
void printString(char *msg){
	write(1,msg,strlen(msg));
}


void stats(int pid) {
	struct stats state;
	get_stats(getpid(), &state);
	char buffer[] = "\n Stats del proces\n";
	printString(buffer);

	escriu_int(pid);
	char buffer2[] =  "    User Ticks: ";
	printString(buffer2);

	escriu_int(state.user_ticks);
	char buffer3[] =   "    System Ticks: ";
	printString(buffer3);

	escriu_int(state.system_ticks);
	char buffer4[] =   "   Blocked Ticks: ";
	printString(buffer4);

	escriu_int(state.blocked_ticks);
	char buffer5[] =    "\nReady Ticks: ";
	printString(buffer5);

	escriu_int(state.ready_ticks);
	char buffer6[] =    "   FINNN\n ";
	printString(buffer6);
}

int Work1() {
	int a = 534;
	int b = 799;
	for (int i = 0; i < 100000000; ++i) {
		a = (a * b) * b * b;

	}
	stats(getpid());
	return a;
}

void Work2() {
	char *buffers;
	read(0, &buffers, 10000);
	stats(getpid());
}

int Work3() {
	char *buffers;
	int a = 534;
	int b = 799;
	for (int i = 0; i < 100000000; ++i) {
		a = (a * b) * b * b;	
	}
	read(0, &buffers, 10000);
	
	stats(getpid());
	return a;
}
void fork1() {
  int a=0;
  int ret = fork();

  if(ret == 0) {
    char buffer3[] = "\nentra a fork1 fill1  ";
    printString(buffer3);
    a += Work3();
    exit();
  }
  ret = -1;
  ret = fork();	
  if(ret == 0) {
    char buffer5[] = "\nentra a fork2 fill1  ";
    printString(buffer5);
    a+= Work3();
    exit();
  }
  ret = -1;
  ret = fork();	
  if(ret == 0) {
    char buffer4[] = "\nentra a fork3 fill1  ";
    printString(buffer4);
    a+= Work3();
    exit();
  }
  exit();
}


int __attribute__ ((__section__(".text.main")))
main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  char buffer[] = "\nBienvenido a Zeos\n";
  printString(buffer);	
  set_sched_policy(0);
  fork1();
  
     
  while(1) { }
}
