CFLAGS= -g -I..

01-main: 01-main.o thread.o

thread.o: ../thread.c
	$(CC) -c $(CFLAGS) $< -o $@