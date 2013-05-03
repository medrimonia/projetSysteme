#include "fifo.h"

#include <glib.h>


struct fifo {
    Glist *head;
    Glist *tail;
    int nb_elements
}

/* Get the first element of the fifo and removes it
 */
thread_t dequeue(struct fifo*) {
    return NULL;
}

/* Get the first element of the fifo without removing it
 */
thread_t head(struct fifo*) {
    return ((head == NULL)?NULL:head->data);
}

/* Insert a thread at the end of the fifo
 */
void queue(struct fifo*, thread_t) {
}
