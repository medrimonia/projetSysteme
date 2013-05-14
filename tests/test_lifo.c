#include "../lifo.h"

#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv) {
    int a = 5;
    int b = 4;
    struct fifo *f = initialize_fifo();
    assert(fifo_size(f) == 0);

    queue(f, &a);
    assert(fifo_size(f) == 1);

    queue(f, &b);
    assert(fifo_size(f) == 2);

    assert(dequeue(f) == &a);
    assert(dequeue(f) == &b);

    free_fifo(f);
    return EXIT_SUCCESS;
}
