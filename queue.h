/*
 * Queue data structure for Proc struct.
 */
#ifndef __QUEUE_H_
#define __QUEUE_H_
 
#include <stdio.h>
#include "proc.h"

extern char *statusNames[];

void Enqueue(Proc **queue, Proc *process);
Proc *Dequeue(Proc **queue);
void PrintList(char *name, Proc *process);

#endif

