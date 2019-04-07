/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;
extern struct list_head free_queue;
extern struct list_head ready_queue;
struct task_struct * idle_task;

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{
	struct list_head *first = list_first(&free_queue);
	list_del(first);
	struct task_struct *p = list_head_to_task_struct(first);
	p -> PID = 0;
	allocate_DIR(p);
	union task_union *tnew = (union task_union*)p;
	tnew->stack[KERNEL_STACK_SIZE-1]=cpu_idle;
	tnew->stack[KERNEL_STACK_SIZE-2]=0;
	p->esp=&tnew->stack[KERNEL_STACK_SIZE-2];
	
	idle_task = p;
}

void init_task1(void)
{
	struct list_head *first = list_first(&free_queue);
	list_del(first);
	struct task_struct *p = list_head_to_task_struct(first);
	p -> PID = 1;
	allocate_DIR(p);
	set_user_pages(p);
	union task_union *nova = (union task_union*) p;
	tss.esp0 = (DWord)&(nova -> stack[KERNEL_STACK_SIZE]);
	(nova -> stack[KERNEL_STACK_SIZE], 0x175);
	set_cr3(get_DIR(&nova->task));
}


void init_sched(){
	INIT_LIST_HEAD( &free_queue );
	for(i = 0; i < NR_TASK; ++i) {
		list_add_tail(&(task[i].task.anchor), &free_queue);
	}
	INIT_LIST_HEAD( &ready_queue );
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void task_switch(union task_union *t) {
}

void inner_task_switch(union task_union *t) {
	tss.esp0 = (int)&(t->stack[KERNEL_STACK_SIZE]);
	(&t->stack[KERNEL_STACK_SIZE], 0x175);
	set_cr3(get_DIR(&t->task));
	task_st(&current()->esp, t->task.esp);
}
int quantumt = 0;
void update_sched_data_rr(void) {
	quantumt--;
}

int get_quantum(struct task_struct *t) {
	return t->quantum;
}

void set_quantum(struct task_struct *t, int new_quantum) {
	t->quantum=new_quantum;
}

int needs_sched_rr(void) {
	if (quantumt == 0) {
		if (list_empty(&ready_queue)) {
			quantumt = current()-> quantum;
			return 0;
		}
		else {
			return 1;
		}
	}
	return 0;
}

void update_process_state_rr(struct task_struct *t, struct list_head *dst_queue) {
	if (t->STATE != ST_RUN) list_del(&(t->list);
	if (dst_queue != NULL) {
		list_add(&(t->list), dst_queue); 
		if (dst_queue == &readyqueue) t->state = ST_READY; 
		else t->state = ST_BLOCKED; 
	}
	else t->state = ST_RUN;
}

void sched_next_rr() {
	struct task_struct *next_task; 
	if(list_empty(&readyqueue)) next_task = idle_task;	
	else { 
		struct list_head *lh = list_first(&readyqueue); 
		next_task = list_head_to_task_struct(lh);	
		list_del(lh);
	}
	quantum_total = get_quantum(next_task);
	next_task->state = ST_RUN; 
	task_switch((union task_union*)next_task); 
}

void schedule() {
	update_sched_data_rr();
	if (needs_sched_rr()) {
		update_process_state_rr(current(), &readyqueue);
		sched_next_rr();
	}
}