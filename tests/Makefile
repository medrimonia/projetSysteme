CFLAGS= -Wall -Wextra -Wno-unused-parameter -g -I.. `pkg-config --cflags --libs glib-2.0`

LIBS=-lglib-2.0 -lpthread

SRC=$(wildcard *.c)

TESTS=$(SRC:.c=.test)

RESULTS=$(SRC:.c=.output)

VALGRINDS=$(SRC:.c=.valgrind)

all: $(RESULTS) $(VALGRINDS) parallel-speed-compare

valgrinds: $(VALGRINDS)

tests: $(TESTS)

%.output: %.test
	./$< > $@

parallel-speed-compare: parallel-speed-compare.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

parallel-speed-compare.o: parallel-speed-compare.c

21-create-many.output: 21-create-many.test
	./$< 50 > $@

22-create-many-recursive.output: 22-create-many-recursive.test
	./$< 50 > $@

31-switch-many.output: 31-switch-many.test
	./$< 50 4 > $@

32-switch-many-join.output: 32-switch-many-join.test
	./$< 50 4 > $@

51-fibonacci.output: 51-fibonacci.test
	./$< 9 > $@

%.valgrind: %.test
	valgrind ./$< > /dev/null 2> $@

21-create-many.valgrind: 21-create-many.test
	valgrind ./$< 50 > /dev/null 2> $@

22-create-many-recursive.valgrind: 22-create-many-recursive.test
	valgrind ./$< 50 > /dev/null 2> $@

31-switch-many.valgrind: 31-switch-many.test
	valgrind ./$< 50 4 > /dev/null 2> $@

32-switch-many-join.valgrind: 32-switch-many-join.test
	valgrind ./$< 50 4 > /dev/null 2> $@

51-fibonacci.valgrind: 51-fibonacci.test
	valgrind ./$< 9 > /dev/null 2> $@

%.test: %.o thread.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

thread.o: ../thread.c
	$(CC) -c $(CFLAGS) $< -o $@

lifo.o: ../lifo.c
	$(CC) -c $(CFLAGS) $< -o $@

test_lifo: test_lifo.c lifo.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)	

clean:
	rm -rf *.o

mrproper: clean
	rm -rf *.test
	rm -rf *.output
	rm -rf *.valgrind
