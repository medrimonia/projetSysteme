#include "thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define NB_THREADS_MAX 100
#define STACK_SIZE 64 * 1024
#define STATUS_TERMINATED 1


struct thread{
  ucontext_t context;
  int status;
  void * retval;
};

/* L'identifiant du thread est mis à jour au fur et à mesure
 */
int current_thread = 0;
struct thread * threads = NULL;
int nb_threads = 0;
int next_thread_create = 0;

struct thread * add_thread(){
  threads[next_thread_create].status = 0;
  threads[next_thread_create].retval = NULL;
  nb_threads++;
  return &threads[next_thread_create++];
}

struct thread * next_thread(){
  int next = current_thread;
  do{
    next++;
    if (next == next_thread_create)
      next = 0;
    if (threads[next].status != STATUS_TERMINATED){
      current_thread = next;
      return &threads[next];
    }
  }while(1);
}

thread_t thread_self(){
  return &threads[current_thread];
}

void initialize_thread_handler(){
  threads = malloc(NB_THREADS_MAX * sizeof(struct thread));
  struct thread * this_thread = add_thread();
}

int thread_create(thread_t * newthread,
                  void *(*func)(void *),
                  void * funcarg){
  if (next_thread_create == 0){
    initialize_thread_handler();
  }
  struct thread * new_thread = add_thread();
  ucontext_t * new_context = &new_thread->context;
  getcontext(new_context);
  new_context->uc_stack.ss_size = STACK_SIZE;
  new_context->uc_stack.ss_sp = malloc(new_context->uc_stack.ss_size);
  new_context->uc_link = NULL;
  makecontext(new_context, (void (*)(void)) func, 1, funcarg);
  *newthread = (void *)new_thread;
  //printf("thread created : %p\n", *newthread);
  return 0;
}

int thread_yield(){
  if (next_thread_create == 0){
    initialize_thread_handler();
  }
  struct thread * my_thread = thread_self();
  struct thread * next = next_thread();
  if (next != my_thread){
    swapcontext(&my_thread->context, &next->context);
  }
  return 0;
}

int thread_join(thread_t thread, void ** retval){
  struct thread * to_wait = (struct thread *) thread;
  while(to_wait->status != STATUS_TERMINATED){
    thread_yield();
  }
  if (retval != NULL)
    *retval = to_wait->retval;
  return 0;
}

void thread_exit(void *retval){
  struct thread * my_thread = thread_self();
  my_thread->status = STATUS_TERMINATED;
  my_thread->retval = retval;
  struct thread * next = next_thread();
  setcontext(&next->context);
}
