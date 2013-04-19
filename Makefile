thread.o: thread.c
	cc `pkg-config --cflags --libs glib-2.0` -Wall -Wextra thread.c -c
