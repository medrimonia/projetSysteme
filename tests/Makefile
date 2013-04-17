CFLAGS= -g -I..

01-main: 01-main.o thread.o
02-switch: 02-switch.o thread.o

thread.o: ../thread.c
	$(CC) -c $(CFLAGS) $< -o $@