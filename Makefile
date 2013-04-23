thread.o: thread.c
	cc `pkg-config --cflags --libs glib-2.0` -Wall -Wextra thread.c -c

example: thread.o example.c
	cc `pkg-config --cflags --libs glib-2.0` -Wall -Wextra thread.o example.c -o example
