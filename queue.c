/*
 * Priority queue data structure for Proc.
 */
#include "queue.h"

/*
 * Insert Proc process into priority queue of Proc's.
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
		proc->next = NULL;
	}
	return proc;
}

/*
 * Print process list.
 */
void PrintList(char *name, Proc *process)
{
	printf("%s [pid, priority] = ", name);
	while(process) {
		printf("[%d %d]->", process->pid, process->priority);
		process = process->next;
	}
	printf("NULL\n");
}

