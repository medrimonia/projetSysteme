#include "fifo.h"

#include <stdlib.h>
#include <assert.h>

int main (int argc, char** argv) {
    struct fifo *f = initialize_fifo();
    assert(fifo_size(f) == 0);
    free_fifo(f);
    return EXIT_SUCCESS;
}
