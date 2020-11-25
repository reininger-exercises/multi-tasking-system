#include "mt.h"

Proc procs[NPROC]; // NPROC Procs
Proc *freeList; // list of FREE Procs
Proc *readyQueue; // priority queue of READY Procs
Proc *running; // current running Proc

/*
 * Initialize the multi-tasking(MT) system, creating P0 as the initial
 * running process. Note that unlike all other processes, the execution image
 * of P0 is the same as the linux process itself.
 */
void init()
{
	// initialize Proc's
	for(int i=0; i<NPROC; i++) {
		procs[i].next = &procs[i+1]; // create freeList
		procs[i].pid = i;
		procs[i].status = FREE;
		procs[i].priority = 0;
	}

	procs[NPROC-1].next = NULL;
	freeList = &procs[0];

	// initialize readyQueue
	readyQueue = NULL;

	// create P0 as initial running process
	Proc *p0 = running = Dequeue(&freeList);
	if (!p0) {
		perror("Cannot create P0, no free processes\n");
		exit(1);
	}

	p0->status = READY;
	p0->ppid = 0;
	PrintList("freeList", freeList);
	printf("init complete: P0 running\n");
}

/*
 * Call kfork() and print diagnostics.
 */
int do_kfork()
{
	int child = kfork();
	if (child < 0) {
		printf("kfork failed\n");
	}
	else {
		printf("proc %d kforked a child pid = %d\n", running->pid, child);
		PrintList("readyQueue", readyQueue);
	}
	return child;
}

/*
 * Call tswitch().
 */
void do_switch()
{
	tswitch();
}

/*
 * Call kexit().
 */
void do_exit()
{
	kexit();
}

/*
 * Print Proc information.
 */
void ProcInfo()
{
	printf("running pid: %d parent pid: %d\n", running->pid, running->ppid);
	PrintList("freeList", freeList);
	PrintList("readyQueue", readyQueue);
}

/*
 * Function executed by all processes except P0. Gets command from user to fork
 * a child, give up the processor, or terminate.
 */
void body()
{
	char input;
	printf("proc %d starts from body()\n", running->pid);
	while(true) {
		printf("***************************************\n");
		printf("proc %d running; parent pid = %d\n", running->pid, running->ppid);
		printf("\033[0;32menter a key [f|s|q|i]: \033[0m");
		input = getchar(); getchar(); // kill the \n
		switch(input) {
			case 'f': do_kfork(); break;
			case 's':
				do_switch();
				printf("resuming body() from proc %d\n", running->pid);
				break;
			case 'q':
				do_exit();
				break;
			case 'i':
				ProcInfo();
				break;
			case EOF:
				putchar('\n');
				exit(0);
				break;
			default:
				printf("Invalid command\n"); break;
		}
	}
}

/*
 * Create child process and return child pid.
 */
int kfork()
{
	// get a FREE process
	Proc *process = Dequeue(&freeList);
	if (!process) {
		printf("No free Proc\n");
		return -1;
	}

	process->status = READY;
	process->priority = 1; // all procs priority = 1 except P0
	process->ppid = running->pid;

	// prepare stack, zero-out saved registers
	for (int i=1; i<10; i++) {
		process->kstack[SSIZE-i] = 0;
	}
	process->kstack[SSIZE-1] = (int)body; // retPC -> body()
	// Proc.ksp -> saved eflag, full descending stack
	process->ksp = &(process->kstack[SSIZE-9]); 
	Enqueue(&readyQueue, process); // enter process into ready queue
	return process->pid;
}

/*
 * Terminate running process.
 */
void kexit()
{
	// free process
	running->status = FREE;
	running->priority = 0;
	Enqueue(&freeList, running);
	PrintList("freeList", freeList);
	tswitch(); // give up processor
}

/*
 * Select next process to run and set as running process.
 */
void scheduler()
{
	printf("proc %d in scheduler()\n", running->pid);
	if (running->status == READY)
		Enqueue(&readyQueue, running);
	PrintList("readyQueue", readyQueue);
	running = Dequeue(&readyQueue);
	printf("next running = %d\n", running->pid);
}

