CFLAGS= -g -I..

01-main: 01-main.o thread.o

02-switch: 02-switch.o thread.o

11-join: 11-join.o thread.o

12-join-main: 12-join-main.o thread.o

21-create-many: 21-create-many.o thread.o

22-create-many-recursive: 22-create-many-recursive.o thread.o

31-switch-many: 31-switch-many.o thread.o

32-switch-many-join: 32-switch-many-join.o thread.o

51-fibonacci: 51-fibonacci.o thread.o

thread.o: ../thread.c
	$(CC) -c $(CFLAGS) $< -o $@