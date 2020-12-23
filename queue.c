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
char* statusNames[] = {"FREE", "READY", "SLEEP", "ZOMBIE"};
void PrintList(char *name, Proc *process)
{
	printf("%s [pid, status] = ", name);
	while(process) {
		printf("[%d %s]->", process->pid, statusNames[process->status]);
		process = process->next;
	}
	printf("NULL\n");
}

