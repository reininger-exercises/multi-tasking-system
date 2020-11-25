/*
 * Reid Reininger
 * charles.reininger@wsu.edu
 * Define process control block structure (PCB).
 * The goal of the program is to simulate process management using a linux
 * process as the virtual CPU. The program, running inside a real process,
 * will multiplex executing tasks within itself. To the processes inside the
 * program, the linux process is the CPU.
 *
 * Adapted from KC Wang's "Systems Programming in Unix/Linux", 2018.
 */

#ifndef __TYPE_H_
#define __TYPE_H_

#include <stdbool.h>

#define NPROC 8 // number of Proc structures
#define SSIZE 1024 // stack size

typedef enum procStatus {
	FREE, READY, SLEEP, ZOMBIE
} ProcStatus;

// Example Process control block structure (PCB).
// Conatains all of the information about a process.
typedef struct proc {
	struct proc *next; // next proc pointer, for arrainging in data structures
	int *ksp; // saved stack pointer: at byte offset 4 in struct
	int pid; // process id
	int ppid; // parent process pid
	ProcStatus status; // Proc status = FREE|READY, etc
	int priority; // scheduling priority, higher priority more important
	int kstack[SSIZE]; // process execution stack
} Proc;

#endif

