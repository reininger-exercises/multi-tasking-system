/*
 * Priority queue functions for Proc.
 */
#include "queue.h"

/*
 * Insert Proc p into priority queue of Proc's.
 */
void Enqueue(Proc **queue, Proc *process)
{
	Proc *cur = *queue;
	if (!cur || process->priority > cur->priority) { 
		*queue = process;
		process->next = cur;
	}
	else {
		while(cur->next && process->priority <= cur->next->priority) {
			cur = cur->next;
		}
		process->next = cur->next;
		cur->next = process;
	}
}

/*
 * Dequeue Proc from priority queue.
 */
Proc *Dequeue(Proc **queue)
{
	Proc *proc = *queue;
	if (proc) {
		*queue = (*queue)->next;
	}
	return proc;
}

/*
 * Print process list.
 */
void PrintList(char *name, Proc *process)
{
	printf("%s = ", name);
	while(process) {
		printf("[%d %d]->", process->pid, process->priority);
		process = process->next;
	}
	printf("NULL\n");
}

