Simulates a uniprocessor multitasking system, using the program process as a
virtual CPU. Each Proc struct contains its own stack space within the program
stack space, which is used to execute different tasks for each process. The
program multiplexes its processing time between the different tasks within the
simulation by manipulating the program counter and stack pointer.

Build mt executable with makefile. Must have gcc-multilib installed for 32-bit
compilation.

Code adapted from KC Wang's "Systems Programming in Unix/Linux", 2018, for
learning purposes.
