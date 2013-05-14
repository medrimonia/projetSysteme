#include "../fifo.h"

#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv) {
    int a = 5;
    struct fifo *f = initialize_fifo();
    assert(fifo_size(f) == 0);

    queue(f, &a);
    assert(fifo_size(f) == 1);
    assert(dequeue(f) == &a);

    free_fifo(f);
    return EXIT_SUCCESS;
}
