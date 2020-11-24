mt: queue.c t.c type.h queue.h t.h ts.s
	gcc -o mt -m32 t.c ts.s queue.c
