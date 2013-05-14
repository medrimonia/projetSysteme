#include "fifo.h"

#include <glib.h>
#include <stdlib.h>


struct fifo{
    GList *head;
    GList *tail;
    int nb_elements;
};

/* Initialize the fifo structure
 */
struct fifo *initialize_fifo() {
    struct fifo *f = malloc(sizeof(struct fifo));
    f->head = NULL;
    f->tail = NULL;
    f->nb_elements = 0;
    return f;
}

/* Get the first element of the fifo and removes it
*/
thread_t dequeue(struct fifo *f) {
    if(f->head == NULL) {
        return NULL;
    }
    else {
        thread_t ret = f->head->data;
        f->head = g_list_remove(f->head, ret);
        f->nb_elements--;
        return ret;
    }
}

/* Get the first element of the fifo without removing it
 * returns the thread at the head of the fifo
 *  or NULL if empty
 */
thread_t head(struct fifo *f) {
    return ((f->head == NULL)?NULL:f->head->data);
}

/* Insert a thread at the end of the fifo
*/
void queue(struct fifo *f, thread_t t) {
    if(f->head == NULL){
        f->head = g_list_append(f->head, t);
        f->head->next = f->tail;
        f->nb_elements++;
    } 
    else {
        f->tail = g_list_append(f->tail, t);
        f->tail = f->tail->next;
        f->nb_elements++;
    }
}

/* Returns the fifo size
 */
int fifo_size(struct fifo* f) {
    return f->nb_elements;
}

/* Frees the fifo
 */
void free_fifo(struct fifo *f) {
    if (f->head != NULL)
        g_list_free(f->head);
    if (f->tail != NULL)
        g_list_free(f->tail);
    free(f);
}
