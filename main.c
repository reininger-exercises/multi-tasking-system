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

#include "mt.h"

int main()
{
	printf("Welcome to the MT Multitasking System!\n");
	printf("Each simulated process has 3 options: \nf: fork a child process\ns: give up the processor\nq: terminate the process\ni: print process info\n\n");
	init(); // initialize system and start P0
	do_kfork(); // kfork P1 into readyQueue
	while(true) {
		printf("P0: switch process\n");
		if (readyQueue) {
			tswitch();
		}
		else {
			printf("No more child processes to run! Exiting.\n");
			return 0;
		}
	}
}
