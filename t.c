#include <stdlib.h>
#include <stdio.h>
#include "type.h"
#include "queue.h"
#include "t.h"

Proc procs[NPROC]; // NPROC Procs
Proc *freeList; // freeList of Procs
Proc *readyQueue; // priority queue of READY Procs
Proc *running; // current running Proc

/*
 * Initialize the multi-tasking(MT) system, creating P0 as the initial
 * running proces.
 */
void init()
{
	// initialize Proc's
	Proc *proc;
	for(int i=0; i<NPROC; i++) {
		procs[i].next = &procs[i+1];
		procs[i].pid = i;
		procs[i].status = FREE;
		procs[i].priority = 0;
	}

	proc[NPROC-1].next = 0;
	freeList = &procs[0];
	readyQueue = NULL;

	// create P0 as initial running process
	Proc *p0 = running = Dequeue(&freeList);
	if (!proc) {
		perror("Cannot create P0, no free processes\n");
		exit(1);
	}

	p0->status = READY;
	p0->ppid = 0;
	PrintList("freeList", freeList);
	printf("init complete: P0 running\n");
}

int do_kfork()
{
	int child = kfork();
	if (child < 0) {
		printf("kfork failed\n");
	}
	else {
		printf("proc %d kforked a child = %d\n", running->pid, child);
		PrintList("readyQueue", readyQueue);
	}
	return child;
}

void do_switch()
{
	tswitch();
}

void do_exit()
{
	kexit();
}

void body()
{
	char c;
	printf("proc %d starts from body()\n", running->pid);
	while(true) {
		printf("***************************************\n");
		printf("proc %d running: parent=%d\n", running->pid, running->ppid);
		printf("enter a key [f|s|q]: ");
		c = getchar(); getchar(); // kill the \r key
		switch(c) {
			case 'f': do_kfork(); break;
			case 's': do_switch(); break;
			case 'q': do_exit(); break;
		}
	}
}

/*
 * Create child process and return child pid.
 */
int kfork()
{
	Proc *process = Dequeue(&freeList);
	if (!process) {
		printf("No free Proc");
		return -1;
	}

	process->status = READY;
	process->priority = 1; // all procs priority = 1 except P0
	process->ppid = running->pid;

	// prepare stack
	for (int i=1; i<10; i++) {
		process->kstack[SSIZE-i] = 0;
	}
	process->kstack[SSIZE-1] = (int)body; // retPC -> body()
	process->ksp = &(process->kstack[SSIZE-9]); // Proc.ksp -> saved eflag
	Enqueue(&readyQueue, process); // enter process into ready queue
	return process->pid;
}

void kexit()
{
	running->status = FREE;
	running->priority = 0;
	Enqueue(&freeList, running);
	PrintList("freeList", freeList);
	tswitch();
}

void scheduler()
{
	printf("proc %d in scheduler()\n", running->pid);
	if (running->status = READY)
		Enqueue(&readyQueue, running);
	PrintList("readyQueue", readyQueue);
	running = Dequeue(&readyQueue);
	printf("next running = %d\n", running->pid);
}

int main()
{
	printf("Welcom to the MT Multitasking System\n");
	init(); // initialize system and start P0
	kfork(); // kfork P1 into readyQueue
	/*
	while(true) {
		printf("P0: switch process\n");
		if (readyQueue) {
			tswitch();
		}
	}
	*/
}
