mt: queue.c mt.c proc.h queue.h mt.h ts.s main.c
	gcc -g -o mt -m32 mt.c ts.s queue.c main.c
