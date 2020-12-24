#include "mt.h"

Proc procs[NPROC]; // NPROC Procs
Proc *freeList; // list of FREE Procs
Proc *readyQueue; // priority queue of READY Procs
Proc *sleepList; // SLEEP Procs
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
		procs[i].parent = NULL;
		procs[i].child = NULL;
		procs[i].sibling = NULL;
	}
	procs[NPROC-1].next = NULL;

	// initialize Proc lists
	freeList = &procs[0];
	readyQueue = NULL;
	sleepList = NULL;

	// create P0 as initial running process
	Proc *p0 = running = Dequeue(&freeList);
	if (!p0) {
		perror("Cannot create P0, no free processes\n");
		exit(1);
	}

	p0->status = READY;
	p0->ppid = 0;
	p0->parent = p0;
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
 * Call kwait().
 */
void do_wait()
{
	int exitCode;
	int childId = kwait(&exitCode);
	if (childId < 0) {
		printf("proc %d could not wait because it has no children\n", running->pid);
		return;
	}
	printf("proc %d waited for ZOMBIE child pid=%d, status = %d\n", running->pid,
					childId, exitCode);
}

/*
 * Print Proc information.
 */
void ProcInfo()
{
	printf("running pid: %d parent pid: %d\n", running->pid, running->ppid);
	PrintList("freeList", freeList);
	PrintList("readyQueue", readyQueue);
	PrintList("sleepList", sleepList);
	for(int i=0; i<NPROC; i++) {
		PrintChildren(&procs[i]);
	}
}

/*
 * Print children procs of proc.
 */
void PrintChildren(Proc *proc)
{
	Proc *cur = proc->child;
	printf("proc %d children: ", proc->pid);
	while(cur) {
		printf("[%d %s]->", cur->pid, statusNames[cur->status]);
		cur = cur->sibling;
	}
	printf("NULL\n");
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
		PrintChildren(running);
		printf("\033[0;32menter a key [f|s|q|w|i]: \033[0m");
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
			case 'w':
				do_wait();
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

	// Prepare Proc
	process->status = READY;
	process->priority = 1; // all procs priority = 1 except P0
	process->ppid = running->pid;

	// add to proc tree
	process->parent = running;
	process->child = NULL;
	process->sibling = running->child;
	running->child = process;

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
void kexit(int exitCode)
{
	running->exitCode = exitCode;

	// wake parent
	kwakeup((int)running->parent);

	// send orphaned children to P1
	Proc *cur = running->child;
	if (cur) {
		while(cur) {
			cur->parent = &procs[1];
			cur->ppid = procs[1].pid;
			cur = cur->next;
		}
		cur = running->child;
		while(cur->next) cur = cur->next;
		cur->sibling = procs[1].child;
		procs[1].child = cur;
		kwakeup((int)&procs[1]);
	}

	// kill process
	running->status = ZOMBIE;
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

/*
 * Remove process with pid from linked list. Return removed element or NULL if element
 * not in list.
 */
Proc *RemoveListElement(Proc **list, int pid)
{
	Proc *cur = *list, *proc = NULL;
	if (cur && cur->pid == pid) { // if first element
		proc = cur;
		*list = cur->next;
	}

	while(cur && cur->next) { // if not first element
		if (cur->next->pid == pid) {
			proc = cur->next;
			cur->next = proc->next;
		}
	}
	if(proc) proc->next = NULL;
	return proc;
}

/*
 * Put running process to sleep on an event. Will not run until woken up by
 * another process.
 */
void ksleep(int event)
{
	printf("proc %d put to SLEEP\n", running->pid);
	running->event = event;
	running->status = SLEEP;
	Enqueue(&sleepList, running);
	tswitch();
}

/*
 * Wake up all processes sleeping on event.
 */
void kwakeup(int event)
{
	Proc *cur = sleepList;
	while(cur) {
		if (cur->event == event) {
			printf("Woke up proc %d\n", cur->pid);
			RemoveListElement(&sleepList, cur->pid);
			cur->status = READY;
			Enqueue(&readyQueue, cur);
		}
		cur = cur->next;
	}
}

/*
 * Wait for a child process to die. Frees dead Proc and returns pid, places
 * exit code in status. Return -1 if process has no children to wait for.
 */
int kwait(int *status)
{
	while(running->child) {
		Proc *cur = running->child;
		while(cur) {
			if (cur->status == ZOMBIE) {
				// remove from parent's children list
				if(cur == running->child) running->child = cur->sibling;
				else {
					Proc* next = running->child;
					while(next->sibling && next->sibling != cur) {
						next = next->sibling;
					}
					next->sibling = cur->sibling;
				}

				// free proc
				cur->status = FREE;
				cur->parent = NULL;
				cur->sibling = NULL;
				cur->child = NULL;
				cur->priority = 0;
				
				Enqueue(&freeList, cur);
				*status = cur->exitCode;
				return cur->pid;
			}
			cur = cur->sibling;
		}
		ksleep((int)running);
	}
	return -1;
}
