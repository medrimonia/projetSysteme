#ifndef __FIFO_H__
#define __FIFO_H__
#include "thread.h"

struct fifo;

/* Initialize a fifo structure and allocate the memory
 */
struct fifo *initialize_fifo();

/* Get the first element of the fifo and removes it
 */
thread_t dequeue(struct fifo*);

/* Get the first element of the fifo without removing it
 */
thread_t head(struct fifo*);

/* Insert a thread at the end of the fifo
 */
void queue(struct fifo*, thread_t);

/* Returns the fifo size
 */
int fifo_size(struct fifo*);

/* Frees the fifo
 */
void free_fifo(struct fifo*);

#endif
