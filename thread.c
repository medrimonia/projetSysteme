#include "thread.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <valgrind/valgrind.h>

#define NB_THREADS_MAX 100
#define STACK_SIZE 64 * 1024
#define STATUS_TERMINATED 1

void initialize_thread_handler();

struct thread{
  ucontext_t context;
  /* Stack_id is used in order to work properly with valgrind */
  int stack_id;
  /* Status is needed in order to allow join to work properly */
  int status;
  /* retval is used in join and allow to store the return value */
  void * retval;
};

GList *threadss = NULL;
int current_thread = 0;
/* This array of threads will be repaced by a list */
struct thread * threads = NULL;
int nb_threads = 0;
int nb_threads_waiting_join = 0;
int next_thread_create = 0;

struct thread * add_thread(){
  threads[next_thread_create].status = 0;
  threads[next_thread_create].retval = NULL;
  threads[next_thread_create].context.uc_link = NULL;
  threadss = g_list_append(threadss, &threads[next_thread_create]);
  nb_threads++;
  return &threads[next_thread_create++];
}

// switch to next thread
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
/* The wrapper function ensure that the return value will be saved
 * and that the thread will call thread_exit before dying
 */
void wrapper(void *(*func)(void *), void * funcarg){
  struct thread * this_thread = thread_self();
  void * retval = func(funcarg);
  thread_exit(retval);
}

/* Return the address of the current thread, if the thread handler hadn't
 * been initialized, it starts it and return a valid address
 */
thread_t thread_self(){
  if (next_thread_create == 0){
    initialize_thread_handler();
  }
  return &threads[current_thread];
}

/* Allocate memory and initialize everything needed for thread handling
 */
void initialize_thread_handler(){
  threads = malloc(NB_THREADS_MAX * sizeof(struct thread));
  struct thread * this_thread = add_thread();
}

/* Free all the memory used for thread handling
 */
void end_thread_handling(){
  free(threads);
  nb_threads = 0;
  current_thread = 0;
  threads = NULL;
  next_thread_create = 0;
}

/* Create a new thread (initializing the thread handler if necessary)
 * allocate the necessary memory for the new thread stack and then launch
 * a wrapper context which will execute the specified function with the
 * arguments specified.
 */
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
  new_thread->stack_id =
    VALGRIND_STACK_REGISTER(new_context->uc_stack.ss_sp,
                            new_context->uc_stack.ss_sp
                            + new_context->uc_stack.ss_size);
  new_context->uc_link = NULL;
  makecontext(new_context, (void (*)(void)) wrapper, 2, func, funcarg);
  *newthread = (void *)new_thread;
  return 0;
}

/* If other threads are running at the same time, swap context to the next
 * one, otherwise it simply returns
 */
int thread_yield(){
  if (next_thread_create == 0){
    return 0;
  }
  struct thread * my_thread = thread_self();
  struct thread * next = next_thread();
  if (next != my_thread){
    swapcontext(&my_thread->context, &next->context);
  }
  return 0;
}

/* Wait until a certain thread has finished it's job and put the value
 * in retval.
 * If after the join, there's only one thread remaining and no threads
 * waiting to be joined, all the resources used by thread handling are freed.
 * This choice might results with the trouble that an address obtained by a
 * call to thread_self() might not be valid anymore.
 */
int thread_join(thread_t thread, void ** retval){
  struct thread * to_wait = (struct thread *) thread;
  while(to_wait->status != STATUS_TERMINATED){
    thread_yield();
  }
  nb_threads_waiting_join--;
  if (retval != NULL)
    *retval = to_wait->retval;
  if (to_wait != threads){//Stack of first thread shouldn't be freed
    free(to_wait->context.uc_stack.ss_sp);
    VALGRIND_STACK_DEREGISTER(to_wait->stack_id);
  }
  if (nb_threads == 1 && nb_threads_waiting_join == 0)
    end_thread_handling();
  return 0;
}

/* Close the thread currently active, place the appropriate value in retval
 * and let another thread continue it's execution.
 * This function doesn't free the resources used by the stack
 */
void thread_exit(void *retval){
  struct thread * my_thread = thread_self();
  my_thread->status = STATUS_TERMINATED;
  my_thread->retval = retval;
  if (nb_threads == 1)
    exit(EXIT_SUCCESS);
  struct thread * next = next_thread();
  ucontext_t next_context = next->context;
  nb_threads--;
  nb_threads_waiting_join++;
  setcontext(&next_context);
  exit(EXIT_FAILURE);
}
