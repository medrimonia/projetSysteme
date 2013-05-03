#include "fifo.h"

#include <glib.h>


struct fifo {
    Glist *head;
    Glist *tail;
    int nb_elements
}

/* Get the first element of the fifo and removes it
*/
thread_t dequeue(struct fifo *f) {
    if(f->head = NULL) {
        return NULL;
    }
    else {
        thread_t ret = f->head->data;
        f->head = f->head->next;
        g_list_remove(f, ret);
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
        g_list_append(f->head, t);
        f->head->next = f->tail;
        f->nb_elements++;
    } 
    else {
        g_list_append(f->tail, t);
        f->tail = f->tail->next;
        f->nb_elements++;
    }
}
