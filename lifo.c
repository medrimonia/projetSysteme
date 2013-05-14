#include "lifo.h"

#include <stdlib.h>

struct lnk{
    struct lnk *next;
    thread_t data;
};

struct fifo{
    struct lnk *head;
    int nb_elements;
};

/* Initialize the fifo structure
 */
struct fifo *initialize_fifo() {
    struct fifo *f = malloc(sizeof(struct fifo));
    f-> head = NULL;
    f->nb_elements = 0;
    return f;
}

/* Get the first element of the fifo and removes it
*/
thread_t dequeue(struct fifo *f) {
    if(f->nb_elements == 0)
        return NULL;
    thread_t ret = f->head->data;
    struct lnk *to_free = f->head;
    f->head = f->head->next;
    free(to_free);
    return ret;
}

/* Get the first element of the fifo without removing it
 * returns the thread at the head of the fifo
 *  or NULL if empty
 */
thread_t head(struct fifo *f) {
    return (f->head == NULL)?NULL:f->head;
}

/* Insert a thread at the end of the fifo
*/
void queue(struct fifo *f, thread_t t) {
    struct lnk *to_add = malloc(sizeof(struct lnk));
    to_add->data = t;
    if(f->nb_elements == 0) {
        to_add->next = NULL;
        f->head = to_add;
    }
    else {
        to_add->next = f->head->next;
        f->head->next = to_add;
    }
    f->nb_elements++;
}

/* Returns the fifo size
*/
int fifo_size(struct fifo* f) {
    return f->nb_elements;
}

/* Frees the fifo
*/
void free_fifo(struct fifo *f) {
    while(f->head != NULL) {
        struct lnk* to_free = f->head;
        f->head = f->head->next;
        free(to_free);
    }
    free(f);
}
