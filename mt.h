/*
 * Reid Reininger
 * charles.reininger@wsu.edu
 *
 * The goal of the program is to simulate process management using a linux
 * process as the virtual CPU. The program, running inside a real process,
 * will multiplex executing tasks within itself. To the processes inside the
 * program, the linux process is the CPU.
 *
 * Adapted from KC Wang's "Systems Programming in Unix/Linux", 2018.
 */

#ifndef __T_H_
#define __T_H_

#include <stdlib.h>
#include <stdio.h>
#include "proc.h"
#include "queue.h"

extern Proc *readyQueue;

void init();
int do_kfork();
void do_switch();
void do_exit();
void do_wait();
void body();
int kfork();
void kexit();
void Scheduler();
int kwait(int*);
void ksleep(int);
void kwakeup(int);
void PrintChildren(Proc *proc);


// defined in ts.s
void tswitch();

#endif
