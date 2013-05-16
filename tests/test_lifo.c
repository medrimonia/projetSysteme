#include "../lifo.h"
#include "../thread.h"

#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv) {
    thread_t a = (void *)5;
    thread_t b = (void *)4;
    struct fifo *f = initialize_fifo();
    assert(fifo_size(f) == 0);

    queue(f, a);
    assert(fifo_size(f) == 1);

    queue(f, b);
    assert(fifo_size(f) == 2);

    assert(dequeue(f) == a);
    assert(dequeue(f) == b);

    free_fifo(f);
    return EXIT_SUCCESS;
}
